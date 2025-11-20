// Pequeña utilidad para dibujar cells (canvas) de forma simple
export const Renderer = {
  clearGridCell(dom) {
    dom.querySelectorAll('.cell').forEach((c) => {
      c.dataset.color = 0;
      c.style.backgroundColor = '#000';
      if (c.tagName === 'CANVAS') {
        const ctx = c.getContext('2d');
        ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
      }
    });
  },
  createGrid(elem, gridW, gridH, pixel) {
    elem.innerHTML = '';
    for (let row = 0; row < gridH; row++) {
      for (let col = 0; col < gridW; col++) {
        const cell = document.createElement('CANVAS');
        cell.width = pixel; cell.height = pixel;
        cell.dataset.x = col; cell.dataset.y = row;
        cell.className = 'cell';
        elem.append(cell);
      }
    }
    this.clearGridCell(elem);
  },
  drawSprite(ctx, sprite, colorLookup, spriteSize = 8) {
    
    const pixelSize = ctx.canvas.width / spriteSize;
    ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
    sprite.forEach((row, y) => {
      row.forEach((colorId, x) => {
        //if (colorId !== 0) {
          ctx.fillStyle = colorLookup(colorId);
          ctx.fillRect(x * pixelSize, y * pixelSize, pixelSize, pixelSize);
        //}
      });
    });
  }
};