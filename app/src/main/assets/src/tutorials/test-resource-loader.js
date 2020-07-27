var task_id = null;

this.onMessage = function(id, title, target, message) {

    log.info("[test-resource-loader][MESSAGE]", id, title, target, message);

    switch(title) {

    case "task_on_start": {
        task_id = target;
        kenan_api_task.send_message("main_task", "task_on_start", task_id + "[OK]");
        break;
    }
    case "user_message": {
        break;
    }
    }

}

log.info("test-resource-loader.js onload");