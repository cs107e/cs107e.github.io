---
title: Guide to automated checker
---

You can learn more about automated testing, and check on its results
before you make a pull request, by logging into
[TravisCI](https://travis-ci.com) with your GitHub account.  Authorize
Travis CI for Private Projects, giving it access to your private assignments
repositories.
On the left hand side, click on cs107e under the Organizations list.

<img title="TravisCI Home Page" src="../images/travis-organization.png" width="800">

You should now see your assignments repository listed below the filter
repositories search input.

<img title="Assignments Repository" src="../images/travis-ci-student-repo.png" width="800">

Clicking on the repository will show you the results of your latest Travis
build. Here you can scan the Job log to understand any failures reported by the
automated checker. You can also select the Build History tab to view a list of
your past builds.

<img title="CI page for assignments repo." src="../images/travis-history.png" width="800">

On the GitHub, you can also click the Details link in the test result in your
pull request (which you might want to make early -- you can always push more
commits to it) to see the TravisCI results page from testing your latest
solution.

<img title="Pull request page." src="/assignments/assign0/images/07-pull-request.png" width="800">

You can also click the icon next to a commit to see the results of a Travis
build. This icon will show a red "x" for a build failure, a yellow circle for a
build in progress, and a green checkmark for a successful build.
<img title="Pull request page." src="../images/travis-github-commit-icon.png" width="800">

The build page shows the result of testing one particular commit you pushed.
You'll need to scroll down to the log to the line that says `tests/run.sh` and
dig around to find the output if something like `make` failed.  Here's an
example of not including a `background.txt` in your assign0 submission.

<img title="Travis assertion failure" src="../images/travis-assertion.png"
width="800">
