export function createAbout() {

    let content = document.querySelector("#content");
    content.innerHTML = "";

    const header = document.createElement("h1");
    header.textContent = "ABOUT";

    const aboutHeader = document.createElement("h2");
    aboutHeader.textContent = "Our Story – How the Metal Feast Began"

    const paragraph1 = document.createElement("p");
    paragraph1.innerHTML = `
        Black Lounge was born from the simple idea that food should be as loud and unapologetic as the music we love. It all started when [Founder’s Name], a die-hard metalhead and food fanatic, decided that regular hot dogs were too plain for his rebellious taste. After years of late-night gigs, smoky BBQ experiments, and backyard “food mosh pits,” the ultimate creation was born – a menu of hot dogs that shred just like a killer guitar solo.
        <br><br>
        In not so long ago, Black Lounge opened its doors with one mission:
        to bring bold, face-melting flavors to everyone who lives life loud.
        <br><br>
        Our restaurant is a tribute to the energy of live metal shows – the walls shake, the grills roar, and the food is always cranked up to 11. From day one, we’ve welcomed metal fans, food lovers, and anyone looking for something unforgettable.
        <br><br>
        Because here, every hot dog is a headliner.
    `

    content.appendChild(header);
    content.appendChild(aboutHeader);
    content.appendChild(paragraph1);
}
