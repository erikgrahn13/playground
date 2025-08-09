import "./style.css"
import { Project } from "./project";

let defaultProject = new Project();

export default function createSideBar() {

    let sideBar = document.querySelector(".side-bar");

    let addTaskButton = document.createElement("div")
    addTaskButton.setAttribute("class", "add-task");
    addTaskButton.style.borderRadius = "10px"
    let button = document.createElement("div");
    button.style.backgroundColor = "red";
    button.style.padding = "6px 12px 6px 12px";
    button.innerHTML = "+";
    button.style.borderRadius = "20px";

    let buttonLabel = document.createElement("label");
    // buttonLabel.style.padding = "10px";
    buttonLabel.innerHTML = "Add task"

    addTaskButton.appendChild(button);
    addTaskButton.appendChild(buttonLabel);

    sideBar.appendChild(addTaskButton);

    let sideBarHeader = document.createElement("h2");
    sideBarHeader.innerHTML = "My Projects";

    sideBar.appendChild(sideBarHeader);

    addTaskButton.addEventListener("mouseover", () => {
        addTaskButton.style.backgroundColor = "yellow";
    });

    addTaskButton.addEventListener("mouseout", () => {
        addTaskButton.style.backgroundColor = "lightyellow";
    });

    addTaskButton.addEventListener("mousedown", () => {
        addTaskButton.style.backgroundColor = "darkorange";
    });

    addTaskButton.addEventListener("mouseup", () => {
        addTaskButton.style.backgroundColor = "yellow";

    })

}