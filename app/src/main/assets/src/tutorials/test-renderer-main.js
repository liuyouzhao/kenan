kenan_api_script.include("src/tutorials/test-renderer-spirit.js");
var resourceManager = {};

var i = 0;
var EN_EFFECT_STATE =
{
    GL2D_BLUR             : 17,   // blur effect
    GL2D_DARK             : 18,   // dark effect
    GL2D_BI               : 19,   // change to white black map
    GL2D_NORMAL_NOTEXTURE : 20,
    GL2D_NORMAL           : 21
};
var eff = orc2d.Effect_Load(EN_EFFECT_STATE.GL2D_NORMAL);
var effBlur = orc2d.Effect_Load(EN_EFFECT_STATE.GL2D_BI);
log.info("effect is: " + eff);
var zazaka1 = kenan_api_graphics.spirit_create("image_id_zazaka001_sp", -1, 0, 0, 1, 1, 32, 32, 1);
var zazaka2 = kenan_api_graphics.spirit_create("image_id_zazaka002_sp", -1, 0, 0, 1, 1, 32, 32, 1);
kenan_api_graphics.spirit_set_color(zazaka1, 1.0, 1.0, 1.0, 1.0, 0);
kenan_api_graphics.spirit_set_color(zazaka1, 1.0, 1.0, 1.0, 1.0, 1);
kenan_api_graphics.spirit_set_color(zazaka1, 1.0, 1.0, 1.0, 1.0, 2);
kenan_api_graphics.spirit_set_color(zazaka1, 1.0, 1.0, 1.0, 1.0, 3);
kenan_api_graphics.spirit_set_color(zazaka2, 0.2, 0.2, 0.2, 0.5, 0);
kenan_api_graphics.spirit_set_color(zazaka2, 0.2, 0.2, 0.2, 0.5, 1);
kenan_api_graphics.spirit_set_color(zazaka2, 0.2, 0.2, 0.2, 0.5, 2);
kenan_api_graphics.spirit_set_color(zazaka2, 0.2, 0.2, 0.2, 0.5, 3);

var rot = 0.001;
var offset = 4;
var margin = 64;
var frameCount = 0;
this.onFrame = function() {

    orc2d.Gfx_SetClearColor(0.8, 0.8, 0.8, 1.0);
    orc2d.Gfx_Clear();
    orc2d.Effect_Active(eff, true);

    for(var i = 0; i < kenan_api_graphics.screen_get_height() / margin; i ++) {
        for(var j = 0; j < kenan_api_graphics.screen_get_height() / margin; j ++) {
            kenan_api_graphics.spirit_render_ex(zazaka2, j * margin, i * margin, i + rot);
            rot += 0.00001;
        }
    }

    for(var i = 0; i < kenan_api_graphics.screen_get_height() / margin; i ++) {
        for(var j = 0; j < kenan_api_graphics.screen_get_height() / margin; j ++) {
            kenan_api_graphics.spirit_render_ex(zazaka1, j * margin, i * margin, i + rot);
            rot += 0.00001;
        }
    }

    orc2d.Gfx_SwapBuffer();

    var bc = orc2d.Gfx_GetBatchCount();

    if((frameCount ++) >= 600) {
        log.info("BatchCount: " + bc);
        frameCount = 0;
    }

}

kenan_api_task.start("test-resource-loader-01", "src/tutorials/test-resource-loader.js");
kenan_api_task.start("test-resource-loader-02", "src/tutorials/test-resource-loader.js");
kenan_api_task.start("test-resource-loader-03", "src/tutorials/test-resource-loader.js");

function bindSpiritImage(imageId, spiritId) {
    var w = kenan_api_graphics.image_get_width(imageId);
    var h = kenan_api_graphics.image_get_height(imageId);
    var hTex = kenan_api_graphics.image_get_texture("image_id_zazaka001");

    if(spiritId == zazaka2) {
        kenan_api_graphics.spirit_resize(spiritId, w, h);
        kenan_api_graphics.spirit_set_hot_spot(spiritId, w * 0.5 - offset, h * 0.5 - offset);
        kenan_api_graphics.spirit_set_texture(spiritId, hTex);
    }
    else {
        kenan_api_graphics.spirit_resize(spiritId, w, h);
        kenan_api_graphics.spirit_set_hot_spot(spiritId, w * 0.5, h * 0.5);
        kenan_api_graphics.spirit_set_texture(spiritId, hTex);
    }

    log.info("bindSpiritImage: " + imageId + ", " + spiritId + " hTex:" + hTex + ", w h: " + w + "," + h);
}

this.onMessage = function(id, title, target, message) {
    switch(title) {
    case "user_message": {
        log.info("[task-main]", id, title, target, message);
        if(target == "image_on_load") {
            var image_id = message;
            bindSpiritImage(image_id, image_id + "_sp");
        }
        break;
    }
    }
}

