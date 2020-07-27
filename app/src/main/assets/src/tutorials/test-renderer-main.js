kenan_api_script.include("src/tutorials/test-renderer-spirit.js");

this.onEvent = function(title, arg0, arg1, arg2, arg3) {
    log.info("onEvent " + title, arg0, arg1, arg2, arg3);
    switch(title) {
        case "graphics_image_onload":
            switch(arg0) {
                case "success":
                log.info("load image success " + arg1);
                var img = resourceManager[arg1];
                log.info("image is", img);
                if(img.onload)
                    img.onload();
                break;
                case "failed":
                break;
            }
        break;
    }
}

var x = 0;
var y = 0;
this.onFrame = function() {

    orc2d.Gfx_SetClearColor(0.2, 0.2, 0.2, 1.0);
    orc2d.Gfx_Clear();
    orc2d.Effect_Active(eff, true);

    spr.RenderEx(x + 100, y, 1.47, 1.0);
    spr.RenderEx(x + 200, y, 1.47, 1.0);
    spr.RenderEx(x + 300, y, 1.47, 1.0);

    spr2.RenderEx(x + 100, y + 200, 0, 1.0);
    spr2.RenderEx(x + 200, y + 200, 0, 1.0);
    spr2.RenderEx(x + 300, y + 200, 0, 1.0);

    x ++;
    if(x >= 500) {
        x = 0;
    }
    orc2d.Gfx_SwapBuffer();
}

kenan_api_task.start("test-resource-loader-01", "src/tutorials/test-resource-loader.js");
kenan_api_task.start("test-resource-loader-02", "src/tutorials/test-resource-loader.js");
kenan_api_task.start("test-resource-loader-03", "src/tutorials/test-resource-loader.js");

this.onMessage = function(id, title, target, message) {
    log.info("[task-main]", id, title, target, message);
}