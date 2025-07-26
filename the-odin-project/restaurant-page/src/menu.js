export function createMenu() {
    let content = document.querySelector("#content");
    content.innerHTML = "";


    const header = document.createElement("h1");
    header.textContent = "MENU";

    const menu1Header = document.createElement("h2");
    menu1Header.textContent = "1. Slayer Sausage";

    const menu1 = document.createElement("p");
    menu1.textContent = `
        A dog so intense it’ll melt your face like a killer guitar solo. Stacked with fiery chili, jalapeños, molten cheese, and a drizzle of “Hellfire” hot sauce. Warning: not for the faint of heart.
    `

    const menu2Header = document.createElement("h2");
    menu2Header.textContent = "2. Mosh Pit Madness";

    const menu2 = document.createElement("p");
    menu2.textContent = `
        Everything we could find from the kitchen mosh pit – bacon, crispy onions, BBQ sauce, cheddar, and a secret crunch you’ll have to survive to find out. Every bite is pure chaos (in a good way).
    `

    const menu3Header = document.createElement("h2");
    menu3Header.textContent = "3. The Metallica Melt";

    const menu3 = document.createElement("p");
    menu3.textContent = `
        A hot dog buried under a tidal wave of melted cheese, caramelized onions, and smoky BBQ pulled pork. One bite and you’ll be screaming “Nothing Else Matters.”
    `


    content.appendChild(header);
    content.appendChild(menu1Header);
    content.appendChild(menu1);
    content.appendChild(menu2Header);
    content.appendChild(menu2);
    content.appendChild(menu3Header);
    content.appendChild(menu3);
}
