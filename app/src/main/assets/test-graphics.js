var zazaka_image_id = "image_id_zazaka001";

kenan_api_graphics.image_create(zazaka_image_id);

var imageZazaka = {};
imageZazaka.onload = function() {
    var imgInfo = {};
    imgInfo.width = kenan_api_graphics.image_get_width(zazaka_image_id);
    imgInfo.height = kenan_api_graphics.image_get_height(zazaka_image_id);
    var hTex = kenan_api_graphics.image_get_texture(zazaka_image_id);
    imgInfo.tex = hTex;
    log.info("imageZazaka.onload", imgInfo);
    spr2.Resize(img.width, img.height);
    spr2.SetTexture(hTex);
}

resourceManager[zazaka_image_id] = imageZazaka;
kenan_api_graphics.image_set_src(zazaka_image_id, "res/images/zazaka.png", true);