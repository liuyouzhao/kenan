this.onMessage = function(id, title, target, message) {
    log.info("[test-resource-loader][MESSAGE]", id, title, target, message);
}

log.info("test-resource-loader.js onload");