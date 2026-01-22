const gfx = [
  [
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
  ],
  [
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 1, 0, 1, 0, 1, 1],
    [1, 1, 0, 1, 0, 1, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 1],
    [1, 1, 0, 1, 0, 1, 0, 1],
    [1, 0, 1, 0, 1, 0, 1, 1],
    [1, 1, 0, 1, 0, 1, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1],
  ],
  [
    [1, 1, 1, 1, 1, 1, 0, 0],
    [1, 0, 1, 1, 1, 0, 1, 0],
    [1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 1, 1, 1, 1, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1],
  ]

];

const palette = [
  ["#fff", "#333"],
  ["#f00", "#00f"],
  ["black", "white"],
  ["#aaa", "#0f0"],
];
const PIXEL=4
const engine = new Engine(160*PIXEL, 120*PIXEL);

var player = null;

function boot() {
  engine.sizePixel = PIXEL;
  player = {
    id: 1,
    size: { w: 1, h: 1 }, 
    position: { x: 100, y: 100 },
  };
};

function update() {
  player.position.x = (player.position.x + PIXEL/2) % (160*PIXEL);
  player.position.y = (player.position.y + PIXEL/2) % (120*PIXEL);
}
function render() {
  const { x, y } = player.position;
  engine.Clear();
  engine.Draw(player.id, x, y);
}
