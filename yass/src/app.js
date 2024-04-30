// JavaScript code from dashboard.html
let uploadedData = null; // Store uploaded data globally
const DEBUG = false;

async function debug_init() {
  uploadedData = await fetch("./example_grade.json").then((res) => res.json());
  updateUI(uploadedData.grades);
}

document.addEventListener("DOMContentLoaded", () => {
  if (DEBUG) {
    debug_init();
  }
  document
    .getElementById("fileInput")
    .addEventListener("change", handleFileSelect, false);
});

function handleFileSelect(event) {
  const reader = new FileReader();
  reader.onload = async (e) => {
    try {
      uploadedData = JSON.parse(e.target.result); // Store uploaded data
      updateUI(uploadedData.grades);
    } catch (error) {
      console.error("Error loading JSON file:", error);
    }
  };
  reader.readAsText(event.target.files[0]);
}

function updateUI(assignmentsData) {
  let dashboardRowsHTML = generateDashboardRowsHTML(assignmentsData);
  document.querySelector(".dashboard_table").innerHTML =
    `<tr class="table_header">
          <th style="width: 5em">Assn</th>
          <th style="width: 10em">Submitted</th>
          <th style="width: 10em">Extension</th>
          <th style="width: 10em">Code Review</th>
          <th style="width: 10em">Results</th>
          <th style="width: 17em">Extension Result</th>
        </tr>${dashboardRowsHTML}`;
}

function generateDashboardRowsHTML(assignmentsData) {
  return Object.entries(assignmentsData)
    .map(([assn, assignmentDetails], index) => {
      return generateSingleRowHTML(assn, assignmentDetails, index);
    })
    .join("");
}

function generateSingleRowHTML(assn, assignmentDetails, index) {
  const { name, timeliness, test_totals, test_results, review } =
    assignmentDetails;
  const { submit, extension, retests } = timeliness || {};
  const submitStatus = submit?.status || "No Submission";
  const extensionStatus = extension?.status || "No Extension";
  const codeQuality = findTestResult(test_results, "Quality", "Style");
  const testQuality = findTestResult(test_results, "Quality", "Tests");
  const testTotals = generateTestTotals(test_totals);
  const extensionResults = generateExtensionResults(test_results);

  return `<tr class="table_row ${index % 2 === 0 ? "even" : "odd"}">
            <td><a href="#" onclick="showAssignmentDetails('${assn}');">${assn}</a></td>
            <td>${
              submit
                ? `${submit.when}<br><b>${submitStatus}</b>`
                : "No Submission"
            }</td>
            <td>${
              extension
                ? `${extension.when}<br><b>${extensionStatus}</b>`
                : "No Extension Found"
            }</td>
            <td>Style: <b>${codeQuality}</b><br>Tests: <b>${testQuality}</b></td>
            <td>${testTotals}</td>
            <td>${extensionResults}</td>
        </tr>`;
}

function findTestResult(test_results, testClass, testCategory) {
  return (
    Object.values(test_results).find(
      (test) => test.class === testClass && test.category === testCategory,
    )?.result || "<b>None</b>"
  );
}

function generateTestTotals(test_totals) {
  return (
    Object.entries(test_totals)
      .map(([category, total]) => {
        return `${category}: <b>${total.passed}/${total.total}</b>`;
      })
      .join("<br>") || "<b>None</b>"
  );
}

function generateExtensionResults(test_results) {
  return (
    Object.values(test_results)
      .filter((test) => test.class === "Extension")
      .map((test) => {
        return `${test.extension_name}: <b>${test.result}</b><br>`;
      })
      .join("") || "<b>None</b>"
  );
}
// JavaScript code from assignment.html
async function showAssignmentDetails(assignmentKey) {
  if (!uploadedData) {
    console.error("No data uploaded");
    return;
  }
  const assignmentDetails = uploadedData.grades[assignmentKey];
  if (!assignmentDetails) {
    console.error("Assignment details not found");
    return;
  }
  updateAssignmentUI(assignmentDetails, assignmentKey);
  //   document.getElementById("dashboard").style.display = "none";
  document.getElementById("assignmentDetails").style.display = "block";
}

function updateAssignmentUI(assignmentDetails, assignmentKey) {
  const { name, timeliness, test_totals, test_results, review } =
    assignmentDetails;
  const { submit, extension, retests } = timeliness || {};

  populateFailedTestsSection(test_results, assignmentKey);
  populateReviewSection(review, assignmentKey);
}

function populateFailedTestsSection(test_results, assignmentKey) {
  document.getElementById("failedTestsHeader").textContent =
    `You passed all ${assignmentKey} tests! 🥳`;
  const failedTests = generateFailedTestsContent(test_results, assignmentKey);

  if (failedTests !== "None") {
    document.getElementById("failedTestsHeader").textContent =
      `Tests for ${assignmentKey}`;
    document.querySelector(".details_table").innerHTML =
      `<tr class="table_header">
        <th style="width: 12em">Test ID</th>
        <th style="width: 5em">Status</th>
        <th style="width: 5em">Issue</th>
        <th style="width: 15em">Explanation</th>
        </tr> ${failedTests}`;
  } else {
    document.querySelector(".details_table").innerHTML = "";
  }
}

function generateFailedTestsContent(test_results, assignmentKey) {
  return (
    Object.entries(test_results)
      .filter(
        ([_, test]) =>
          (test.class !== "Quality" &&
            test.class !== "Extension" &&
            !test.result) ||
          test.class === "FullSystem",
      )
      .map(
        ([test_id, test]) => `<tr>
        <td>${test_id}</td>
        <td>${test.result ? "Passed" : "Failed"}</td>
        <td>${test.issue ? `<a href="${test.issue}" target="_blank">Link</a>` : "None"}</td>
        <td>${test.explanation || "None"}</td>
        </tr>`,
      )
      .join("") || "None"
  );
}

function populateReviewSection(review, assignmentKey) {
  const selector = document.getElementById("fileReviewSelector");
  selector.innerHTML = `<option value="none">No Files</option>`;
  selector.onchange = () => updateReviewCodeMirror(assignmentKey);
  window.updateEditorContent("");
  document.getElementById("editor").style.display = "none";

  if (review && Object.entries(review).length > 0) {
    populateReviewFileSelector(review, selector, assignmentKey);
  }
}

function populateReviewFileSelector(review, selector, assignmentKey) {
  Object.keys(review).forEach((fileName, index) => {
    const option = document.createElement("option");
    option.value = fileName;
    option.textContent = fileName;
    if (index === 0) {
      document.getElementById("editor").style.display = "block";
      selector.innerHTML = ""; // Clear existing options
      selector.value = fileName;
      selector.appendChild(option);
      updateReviewCodeMirror(assignmentKey);
    } else {
      selector.appendChild(option);
    }
  });
}

async function updateReviewCodeMirror(assignmentKey) {
  const assignmentDetails = uploadedData.grades[assignmentKey];
  if (!assignmentDetails) {
    console.error("Assignment details not found");
    return;
  }
  const selector = document.getElementById("fileReviewSelector");
  const selectedIndex = selector.value;
  if (selectedIndex !== "") {
    const fileReview = assignmentDetails.review[selectedIndex];
    window.updateEditorContent(fileReview.lines, fileReview.comments);
  }
}
