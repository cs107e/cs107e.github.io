import { EditorView, basicSetup } from "codemirror";
import { cpp } from "@codemirror/lang-cpp";
import {
  EditorState,
  Extension,
  RangeSetBuilder,
  StateEffect,
  StateField,
  Transaction,
} from "@codemirror/state";
import { Decoration, DecorationSet, WidgetType } from "@codemirror/view";

interface EditorComment {
  from: number;
  to: number;
  comment: string;
}

// Custom widget to display the comment
class CommentWidget extends WidgetType {
  content: string;

  constructor(content: string) {
    super();
    this.content = content;
  }

  toDOM() {
    const outerDiv: HTMLElement = document.createElement("div");
    outerDiv.className =
      "MuiPaper-root jss37 jss39 MuiPaper-elevation1 MuiPaper-rounded";

    const innerDiv1: HTMLElement = document.createElement("div");
    innerDiv1.className = "jss24";
    outerDiv.appendChild(innerDiv1);

    const innerDiv2: HTMLElement = document.createElement("div");
    innerDiv1.appendChild(innerDiv2);

    const text: HTMLElement = document.createElement("p");
    text.textContent = this.content; // Changed from innerHTML to textContent for security reasons
    innerDiv2.appendChild(text);

    return outerDiv;
  }
}

//Styling for highliting lines that were commented on
const commentStyle = EditorView.baseTheme({
  "&light .cm-commentHighlight": {
    backgroundColor: "#d4fafa",
  },
  "&dark .cm-commentHighlight": {
    backgroundColor: "#1a2727",
  },
});
const commentHighlight = Decoration.line({
  attributes: { class: "cm-commentHighlight" },
});

// State Field for the decorations to be shown
const commentsField = StateField.define<DecorationSet>({
  create(state: EditorState): DecorationSet {
    return Decoration.none;
  },
  update(decorations: DecorationSet, transaction: Transaction): DecorationSet {
    const builder = new RangeSetBuilder<Decoration>();
    const comments = transaction.state.field(commentDataField, false) || [];
    const doc = transaction.newDoc;

    comments.forEach((comment) => {
      let start_line, end_line;
      try {
        start_line = doc.line(comment.from);
        end_line = doc.line(comment.to);
      } catch (error) {
        // This is a hack to fix the fact that when the student selects a new file
        // codemirror draws a blank editor but tries to add the comment decorations for the previous file
        // and then we get a RangeError when we try to access the line for the comment
        if (error instanceof RangeError) {
          return; // Skip this comment
        } else {
          throw error; // Rethrow if it's not a RangeError
        }
      }
      const deco = Decoration.widget({
        widget: new CommentWidget(comment.comment),
        side: 1,
        block: true,
      });
      for (let line = start_line.number; line <= end_line.number; line++) {
        const currentLine = transaction.newDoc.line(line);
        builder.add(currentLine.from, currentLine.from, commentHighlight);
      }
      builder.add(end_line.to, end_line.to, deco);
    });

    return builder.finish();
  },
  provide: (f) => EditorView.decorations.from(f),
});

// State field and effect for the comment data behind the decorations
const updateCommentsDataEffect = StateEffect.define<EditorComment[]>();
const commentDataField = StateField.define<EditorComment[]>({
  create() {
    return [];
  },
  update(oldState: EditorComment[], transaction: Transaction) {
    let newState = oldState;
    for (let effect of transaction.effects) {
      if (effect.is(updateCommentsDataEffect)) {
        newState = effect.value;
      }
    }
    return newState;
  },
});

// Packaging the fields and style that make up the extension
function codeReview(): Extension {
  return [commentStyle, commentDataField, commentsField];
}

let editor = new EditorView({
  extensions: [basicSetup, cpp(), EditorState.readOnly.of(true), codeReview()],
  parent: document.querySelector("#editor") as Element,
});

// Function to update editor content
function updateEditorContent(newContent: string, comments: EditorComment[]) {
  // Validate that the comments are sorted and are not overlapping so that
  // code mirror doesn't crash
  comments = comments || [];
  comments.sort((a, b) => a.from - b.from);
  let filteredComments = comments.reduce(
    (acc: EditorComment[], current: EditorComment) => {
      const overlap = acc.some(
        (comment) => comment.from <= current.to && comment.to >= current.from
      );
      if (!overlap) {
        acc.push(current);
      }
      return acc;
    },
    [] as EditorComment[]
  );
  editor.dispatch(
    {
      changes: { from: 0, to: editor.state.doc.length, insert: newContent },
    },
    {
      effects: updateCommentsDataEffect.of(filteredComments),
    }
  );
}

// Expose the function globally to the html
(window as any).updateEditorContent = updateEditorContent;
