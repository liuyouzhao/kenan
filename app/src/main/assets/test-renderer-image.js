kenan_api_script.include("test-renderer.js");

this.onload = function(mark) {
    log.info("onload triggered " + mark);
    var img = resourceManager[mark];
    img.onload();
}

this.update = function() {

    orc2d.Gfx_SetClearColor(0.2, 0.2, 0.2, 1.0);
    orc2d.Gfx_Clear();
    orc2d.Effect_Active(eff, true);

    spr.RenderEx(x, y, 1.47, 1.0);

    spr2.RenderEx(x, y + 200, 0, 1.0);


    spr2.RenderEx(400, 200, 1.47, 1.0);

    spr2.RenderEx(100, 400, 1.47, 1.0);
    x ++;
    if(x >= 500)
    {
        x = 0;
    }

    orc2d.Gfx_SwapBuffer();
}