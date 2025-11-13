window.addEventListener("DOMContentLoaded",()=>{
    const _game = {
        var: { player: null },
        _boot: () => {
            engine.loadGFX(gfx)
            engine.setPalettes(palette);
            boot();
        },
        _loop: async () => {
            _game._update();
            _game._render();
            await engine.Wait();
        },
        _update: update,
        _render: render,
    };
    
    (async function () {
        _game._boot();
        while (true) {
            await _game._loop();
        }
    })()
})
