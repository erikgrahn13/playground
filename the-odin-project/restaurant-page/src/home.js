import image from "./background.jpg";

export function createHome() {
    let content = document.querySelector("#content");
    content.innerHTML = "";

    const header = document.createElement("h1");
    header.textContent = "BLACK LOUNGE";

    const imageElement = document.createElement("img");
    imageElement.src = image;

    const paragraph1 = document.createElement("p");
    const paragraph2 = document.createElement("p");
    const paragraph3 = document.createElement("p");
    paragraph1.textContent = `
        Welcome to Black Lounge, where the power of heavy metal meets the glory of the ultimate hot dog! We’re not just serving food – we’re serving headbanging flavor. Every dog on our menu is built to rock your taste buds, with bold toppings, fiery sauces, and combinations as legendary as the greatest guitar riffs.`

    paragraph2.textContent = `Born from a love of both metal culture and comfort food, our kitchen takes the humble hot dog and cranks it up to 11. From our “Hellfire Dog” (loaded with fiery jalapeños and lava sauce) to the “Black Sabbath BBQ Dog” dripping with smoky goodness, every bite is a tribute to epic flavor.`

    paragraph3.textContent = `So grab a seat, bang your head, and feast like a true rock god.
        Because at Black Lounge, metal isn’t just music – it’s on the menu.`

    content.appendChild(header);
    content.appendChild(imageElement);
    content.appendChild(paragraph1);
    content.appendChild(paragraph2);
    content.appendChild(paragraph3);
}
