export class Project {
    constructor(name = "My first project") {
        this.name = name;
        this.tasks = [];
    }

    addTask(task) {
        this.tasks.push(task);
    }

    removeTask(taskIndex) {
        this.tasks.splice(taskIndex, 1);
    }
} 