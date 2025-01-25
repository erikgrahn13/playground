let container = document.querySelector(".container");

let button = document.createElement("button");
button.textContent = "Click me";
document.body.prepend(button);

button.addEventListener("click", () => {
  let number = prompt("How many number of squarers per side?");
  if (number <= 100) {
    let rows = document.querySelectorAll(".row-attribute");
    rows.forEach((row) => {
      row.remove();
    });
    drawGrid(number);
  }
});

function drawGrid(numberOfSquares) {
  for (let j = 0; j < numberOfSquares; j++) {
    let row = document.createElement("div");
    const rowAttribute = document.createAttribute("class");
    rowAttribute.value = "row-attribute";
    row.setAttributeNode(rowAttribute);

    for (let i = 0; i < numberOfSquares; i++) {
      let newDiv = document.createElement("div");
      const attribute = document.createAttribute("class");
      attribute.value = "grid";
      newDiv.setAttributeNode(attribute);
      newDiv.style.opacity = 1.0;
      newDiv.style.background = "white"

      newDiv.addEventListener("mouseenter", () => {
        if (newDiv.style.backgroundColor == "white") {
          let red = Math.floor(Math.random() * 256);
          let green = Math.floor(Math.random() * 256);
          let blue = Math.floor(Math.random() * 256);
          let color = `rgba(${red}, ${green}, ${blue})`;
          newDiv.style.background = color;
          newDiv.style.opacity = 0.1;
        } else {
          let opacity = parseFloat(newDiv.style.opacity);
          if (opacity < 1) {
            newDiv.style.opacity = opacity + 0.1;
          }
        }
      });
      newDiv.addEventListener("mouseleave", () => {});

      row.appendChild(newDiv);
    }
    container.appendChild(row);
  }

}
drawGrid(16);
