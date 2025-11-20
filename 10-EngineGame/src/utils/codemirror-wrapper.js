export function CodeMirrorComponentFactory(textarea, cbOnChange) {
  let editor = CodeMirror.fromTextArea(textarea, {
    mode: 'javascript',
    lineNumbers: true,
    gutters: ['CodeMirror-lint-markers'],
    lint: true
  });
  editor.on('change', () => cbOnChange(editor.getValue()));
  return editor;
}