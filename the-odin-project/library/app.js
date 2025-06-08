const myLibrary = [];


const dialog = document.querySelector("dialog");
let newBookButton = document.querySelector(".new-book");
newBookButton.addEventListener("click", () => {
    dialog.show();
})

const closeButton = document.querySelector(".close-button");
const addButton = document.querySelector(".add-button");


const inputs = document.querySelectorAll("input");

addButton.addEventListener("click", (e) => {
    addBookToLibrary(inputs[0].value, inputs[1].value, inputs[2].value);
    dialog.close();
})

closeButton.addEventListener("click", (e) => {
    e.preventDefault();
    dialog.close();
})

function Book(title, author, year) {
    this.title = title;
    this.author = author;
    this.year = year;
    this.id = crypto.randomUUID();
    this.status = false;
}

 Book.prototype.toggleStatus = function() {
    this.status = !this.status;
    render();
}

function render() {
    let tableData = document.querySelector("#data");
    tableData.innerHTML = "";

    for(let book of myLibrary){
        let tableRow = document.createElement("tr");
        let title = document.createElement("td");
        let author = document.createElement("td");
        let year = document.createElement("td");
        let status = document.createElement("td");

        title.textContent = book.title;
        author.textContent = book.author;
        year.textContent = book.year;
        status.textContent = book.status;

        tableRow.appendChild(title);
        tableRow.appendChild(author);
        tableRow.appendChild(year);
        tableRow.appendChild(status);

        let readStatus = document.createElement("button");
        readStatus.textContent = "Toggle status";
        readStatus.addEventListener("click", (e) => {
            book.toggleStatus();
        })

        tableRow.appendChild(readStatus);


        tableRow.dataset.id = book.id;
        let deleteButton = document.createElement("button");
        deleteButton.textContent = "Delete";
        deleteButton.addEventListener("click", (e) => {
            const index = myLibrary.findIndex(b => b.id === book.id);
            myLibrary.splice(index, 1);
            render();
        })


        tableRow.appendChild(deleteButton);

        tableData.appendChild(tableRow);
    }

}


function addBookToLibrary(title, author, year) {
    myLibrary.push(new Book(title, author, year));
    render();
}
