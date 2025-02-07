function add(a, b) {
  return a + b;
}

function subtract(a, b) {
  return a - b;
}

function multiply(a, b) {
  return a * b;
}

function divide(a, b) {
  if(b === 0)
  {
    operandA = null;
    operandB = null;
    operator = null;
    return "LOSER! Don't divide by zero!"
  }
  return a / b;
}

function operate(a, b, operator) {
  let answer;
  a = Number(a);
  b = Number(b);
  switch (operator) {
    case "+":
      answer = add(a, b);
      break;
    case "-":
      answer = subtract(a, b);
      break;
    case "*":
      answer = multiply(a, b);
      break;
    case "/":
      answer = divide(a, b);
      break;
    default:
      alert("Unsupported operation");
      break;
  }

  return answer;
}

let numbers = document.querySelectorAll(".numbers");
let display = document.querySelector(".display");
let operators = document.querySelectorAll(".operators");
let equal = document.querySelector(".equal");
let buttons = document.querySelectorAll("button");

buttons.forEach((button) => {
  button.addEventListener("mouseover", (event) => {
    button.style.backgroundColor = "orchid";
  });
  button.addEventListener("mouseout", (event) => {
    button.style.backgroundColor = "darkorchid";
  })
});


equal.addEventListener("click", (event) => {
    if (operandA && operandB) {
        let answer = operate(Number(operandA), Number(operandB), operator);
        display.textContent = answer;
        operandA = answer;
        operandB = null;
        operator = null;
        newUserInput = false;
      }
});

operators.forEach((op) => {
    op.addEventListener("click", (event) => {
        if(operator)
        {
            if(operandA && operandB)
            {
              answer = operate(operandA, operandB, operator);
              display.textContent = answer;
              operandA = answer;
            }
            operator = op.textContent;
            operandB = null;
        }
        else
        {
            operator = op.textContent;
        }
    });
});

let clearButton = document.querySelector(".clear");

clearButton.addEventListener("click", (event) => {
  operandA = null;
  operandB = null;
  operator = null;
  display.textContent = "0";
});

numbers.forEach((number) => {
  number.addEventListener("click", (event) => {

    if (!operator) {
      if (!operandA) {
        operandA = number.textContent;
        display.textContent = number.textContent;
      } else {
        operandA += number.textContent;
        display.textContent += number.textContent;
      }
      newUserInput = true;
    } else {
      if (!operandB) {
        operandB = number.textContent;
        display.textContent = number.textContent;
      } else {
        operandB += number.textContent;
        display.textContent += number.textContent;
      }
      newUserInput = true;
    }
  });
});

let decimalButton = document.querySelector(".decimal");
decimalButton.addEventListener("click", (event) => {

  if(!operator)
  {
    if(!operandA)
    {
        operandA = "0";
    }


    if(String(operandA).indexOf(".") < 0)
    {
      operandA += decimalButton.textContent;
      display.textContent = operandA;
      newUserInput = true;
    }
  }
  else
  {
    if(!operandB)
    {
        operandB = "0";
    }
    if(String(operandB).indexOf(".") < 0)
    {
      operandB += decimalButton.textContent;
      display.textContent = operandB;
      newUserInput = true;
    }
  }

});

let backspaceButton = document.querySelector(".backspace");
backspaceButton.addEventListener("click", (event) => {
  if(newUserInput)
  {
    if(!operator)
    {
      if(operandA)
      {
        let tmp = String(operandA).split("");
        tmp.splice(operandA.length - 1, 1);
        operandA = tmp.join("");
        if(operandA.length === 0)
        {
          operandA = null;
          display.textContent = "0";

        }
        else
        {
          display.textContent = operandA;
        }
      }  

    }
    else
      {
        if(operandB)
        {
          let tmp = String(operandB).split("");
          tmp.splice(operandB.length - 1, 1);
          operandB = tmp.join("");
          if(operandB.length === 0)
          {
            operandB = null;
            display.textContent = "0";
    
          }
          else
          {
            display.textContent = operandB;
          }
        }  
    
      }
   }
});


let operandA;
let operandB;
let operator;
let decimal = true;
let newUserInput = true;
