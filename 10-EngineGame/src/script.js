const current = {
  spriteList: 0,
  filename: "",
  gfx: [],
  tile: [],
  code: "",
};
let editor=null;

function Int2Hex(value) {
  let r = value.toString(16);
  return (r.length == 1 ? "0" : "") + r;
}
function range(start = 0, end = 0, step = null) {
  if (step == null) {
    step = end;
    len = start;
    start = 0;
  } else {
    len = parseInt((end - start) / step);
  }
  return Array(len)
    .fill(0)
    .map((_, index) => start + step * index);
}
/////Class
class ColorPickerComponent {
  #dom = null;
  #dataListColor = null;
  constructor(name = "#color-picker") {
    this.#dom = document.querySelector(name);
    this.#dataListColor = [...this.#dom.list.options].map((e) =>
      e.value.toUpperCase()
    );
  }
  toId(color) {
    color = color.toUpperCase();
    return this.#dataListColor.findIndex((c) => c == color);
  }
  toColor(id = 0) {
    return this.#dataListColor[id];
  }
  getColor() {
    return this.#dom.value;
  }
}

class ListSpriteComponent {
  #dom = null;
  #cells = [];
  constructor(name = "#spriteList") {
    this.#dom = document.querySelector(name);
  }

  eventSetPixel(elem) {
    const color = ColorPicker.getColor();
    const cell = elem.target;
    if (cell.classList.contains("cell")) {
      const indexColor = ColorPicker.toId(color);
      cell.dataset.color = indexColor;
      let y = parseInt(cell.dataset.y),
        x = parseInt(cell.dataset.x);
      current.gfx[current.spriteList][y][x] = indexColor;
      cell.style.backgroundColor = color;
      return { x, y, color };
    }
  }
}

const ColorPicker = new ColorPickerComponent("#color-picker");

function tabComponent(dom) {
  let parent = dom.parentNode;
  parent.querySelector(".active").classList.remove("active");
  dom.classList.add("active");

  item = document.querySelector(dom.dataset.tag);
  parent = item.parentNode;
  parent.querySelector(".show")?.classList.remove("show");
  item.classList.add("show");
}
function NavBarComponent(dom) {
  dom.querySelectorAll("[open]").forEach((detail) => {
    if (detail.hasAttribute("open")) {
      detail.removeAttribute("open");
    }
  });
}
function CodeMirrorComponent(textarea) {
  let delay = 0;
  editor = CodeMirror.fromTextArea(textarea, {
    mode: "javascript",
    lineNumbers: true,
    extraKeys: {
      "Ctrl-S": saveFile,
    },
    gutters: ["CodeMirror-lint-markers"],
    lint: true,
  });
  editor.on("change", function () {
    clearTimeout(delay);
    delay = setTimeout(updatePreview, 300);
  });
  function updatePreview() {
    //[... document.querySelectorAll(".CodeMirror-line")].map(x=>x.innerText)
    current.code = editor.getValue();
  }
}
//[... document.querySelectorAll(".CodeMirror-line")].map(x=>x.innerText)
function SpriteGrid(elem) {
  const maxH = parseInt(elem.dataset.gridH);
  const maxW = parseInt(elem.dataset.gridW);
  const pixel = parseInt(elem.dataset.gridPixel);
  for (let row = 0; row < maxH; row++) {
    for (let col = 0; col < maxW; col++) {
      let cell = document.createElement("CANVAS");
      cell.width = pixel;
      cell.height = pixel;
      cell.dataset.x = col;
      cell.dataset.y = row;
      cell.className = "cell";
      elem.append(cell);
    }
  }
  ClearGridCell(elem);
}
function ClearGridCell(dom) {
  dom.querySelectorAll(".cell").forEach((cell) => {
    cell.dataset.color = 0;
    cell.style.backgroundColor = "#000";
  });
}
function SpriteEditorComponent(elem) {
  const color = ColorPicker.getColor();
  const cell = elem.target;
  if (cell.classList.contains("cell")) {
    const indexColor = ColorPicker.toId(color);
    cell.dataset.color = indexColor;
    let y = parseInt(cell.dataset.y),
      x = parseInt(cell.dataset.x);
    current.gfx[current.spriteList][y][x] = indexColor;
    cell.style.backgroundColor = color;
    SpriteListCellModify(current.spriteList, x, y, color);
  }
}

function SpriteEditorCellModify(imageData) {
  const cells = document.querySelectorAll("#spriteEditor .cell");

  for (let index = 0; index < imageData.data.length; index += 4) {
    const r = imageData.data[index];
    const g = imageData.data[index + 1];
    const b = imageData.data[index + 2];
    const a = imageData.data[index + 3];
    color = `#${Int2Hex(r)}${Int2Hex(g)}${Int2Hex(b)}`;
    //let indexColor = dataListColor.findIndex((c) => c == color);
    indexColor = ColorPicker.toId(color.toUpperCase());
    cells[index / 4].dataset.color = indexColor < 0 ? 0 : indexColor;
    cells[index / 4].style.backgroundColor = color;
  }
}

function SpriteListComponent(child) {
  const canvas = child.target;
  if (canvas.tagName == "CANVAS") {
    const ctx = canvas.getContext("2d");
    const imageData = ctx.getImageData(
      0,
      0,
      ctx.canvas.width,
      ctx.canvas.height
    );
    current.spriteList =
      parseInt(canvas.dataset.y) * 128 + parseInt(canvas.dataset.x);
    SpriteEditorCellModify(imageData);
  }
}

function SpriteListCellModify(id, x, y, color) {
  const cell = document.querySelectorAll("#spriteList .cell")[id];
  const ctx = cell.getContext("2d");
  ctx.fillStyle = color;
  ctx.fillRect(x, y, 1, 1);
}

function newFile() {
  const fecha = new Date();
  const fechaFormateada = fecha.toISOString().split("T")[0];
  current.spriteList = 0;
  current.code = `\nfunction update() {\n\n\n\n\n}\n\n\nfunction render() {\n\n\n\n\n}\n`;
  current.gfx = Array(128)
    .fill([])
    .map((_) =>
      Array(8)
        .fill([])
        .map((x) => Array(8).fill(0))
    );
  current.tile = [];
  current.filename = `undefined-${fechaFormateada}.json`;
}

function saveFile() {
  if (!current.filename) {
    saveAs();
  }
  const { filename, gfx, code, tile } = current;
  downloadFile(filename, JSON.stringify({ gfx, code, tile }));
}
function saveAs(e) {
  const fileName = prompt("Nombre del Proyecto");
  if (fileName) {
    current.code = document.querySelector(`textarea[name="code"]`).value;
    current.tile = [];
    debugger;
    downloadFile(
      fileName,
      JSON.stringify({
        gfx: current.gfx,
        code: current.code,
        tile: current.tile,
      })
    );
  }
}

function downloadFile(filename, content) {
  const blob = new Blob([content], { type: "text/plain" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  a.href = url;
  a.download = filename ?? "undefined.json";
  a.click();
  URL.revokeObjectURL(url);
}

function uploadFile(e) {
  const file = e.target.files[0];
  if (!file) return;
  const reader = new FileReader();
  reader.onload = (event) => {
    try {
      let fileProject = JSON.parse(event.target.result);
      //if (!Array.isArray(tabs) || !tabs.length) throw new Error();
      current.code = fileProject.code;
      current.gfx = fileProject.gfx;
      current.tile = fileProject.tile;
      loadProject();
    } catch {
      alert("Archivo JSON no válido.");
    }
  };
  reader.readAsText(file);
}
function loadProject() {
  editor.setValue(current.code);
  current.gfx.forEach((sprite, id) => {
    sprite.forEach((row, y) => {
      row.forEach((color, x) =>
        SpriteListCellModify(id, x, y, ColorPicker.toColor(color))
      );
    });
  });
}
//getEventListeners(document.querySelector("li")).click == undefined
//getEventListeners(document.querySelector(".tabs-container")).click == object
window.addEventListener("DOMContentLoaded", () => {
  newFile();
  document.querySelectorAll(".tabs-container").forEach((item) => {
    item.addEventListener("click", (tab) => {
      const classNameList = tab.target.classList;
      if (classNameList.contains("tab")) {
        tabComponent(tab.target);
      }
    });
    item.querySelector(".tab.active")?.click();
  });
  document.querySelectorAll(".navbar").forEach((item) => {
    item.addEventListener("click", () => {
      NavBarComponent(item);
    });
  });
  //Solo habrá UNO
  document.querySelectorAll(`textarea[name="code"]`).forEach((textarea) => {
    textarea.value = current.code;
    CodeMirrorComponent(textarea);
  });
  document.querySelectorAll(`.grid`).forEach((elem) => SpriteGrid(elem));
  document
    .querySelector("#spriteEditor")
    .addEventListener("click", SpriteEditorComponent);
  document
    .querySelector("#spriteList")
    .addEventListener("click", SpriteListComponent);
  document.querySelector("#newBtn").addEventListener("click", newFile);
  document.querySelector("#saveBtn").addEventListener("click", saveFile);
  document.querySelector("#saveAsBtn").addEventListener("click", saveAs);
  document.getElementById("openBtn").addEventListener("click", () => {
    const fileInput = document.createElement("INPUT");
    fileInput.type = "file";
    fileInput.accept = ".json";
    fileInput.addEventListener("change", uploadFile);
    fileInput.click();
  });
});
