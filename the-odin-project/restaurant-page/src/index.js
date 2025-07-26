import "./styles.css"
import {createHome} from "./home.js";
import {createMenu} from "./menu.js";
import {createAbout} from "./about.js";

createHome();


let homeTab = document.querySelector("#home");
let menuTab = document.querySelector("#menu");
let aboutTab = document.querySelector("#about");

homeTab.addEventListener("click", () => {
    createHome();
})

menuTab.addEventListener("click", () => {
    createMenu();
})

aboutTab.addEventListener("click", () => {
    createAbout();
})
