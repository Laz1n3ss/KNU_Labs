// math
const MaxValue = 100;
const MinValue = 10;
// Generates random integer between MinValue and MaxValue
function randomInt() {
  return Math.round(Math.random() * (MaxValue - MinValue)) + MinValue;
}
function swap(array, i, j) {
  let t = array[i];
  array[i] = array[j];
  array[j] = t;
}
async function sleep(duration) {
  await new Promise(resolve => setTimeout(resolve, duration));
}
// Drawing
const svg = document.getElementById("visualisation");
let boardWidth = svg.viewBox.baseVal.width;
let elementSize;

function animate({timing, draw, duration=1000}) {
  let start = performance.now();

  return new Promise((resolve) => {
    requestAnimationFrame(function animate(time) {
      // timeFraction изменяется от 0 до 1
      let timeFraction = (time - start) / duration;
      if (timeFraction > 1) timeFraction = 1;

      // вычисление текущего состояния анимации
      let progress = timing ? timing(timeFraction) : timeFraction;

      draw(progress); // отрисовать её

      if (timeFraction < 1) {
        requestAnimationFrame(animate);
      }
      else {
        resolve();
      }
    });
  })
}

let bounds = [];
let blocks = [];
async function handleSteps(steps, speed) {
  console.log("Steps: ", steps);
  for(let i = 0;i < steps.length;++i) {
    let step = steps[i];

    if(step.type == SortStepTypes.COMPARE) {
      await animate({
        draw: (tf) => {
          let value = `rgb(${255 - tf * 255},0,${tf * 255})`;
          blocks[step.first].setAttribute("fill", value);
          blocks[step.second].setAttribute("fill", value);
        },
        duration: 400
      });
      await sleep(400 / speed);
      await animate({
        draw: (tf) => {
          let value = `rgb(${tf * 255},0,${255-tf * 255})`;
          blocks[step.first].setAttribute("fill", value);
          blocks[step.second].setAttribute("fill", value);
        },
        duration: 200
      });
    }
    else if(step.type == SortStepTypes.SWAP) {
      await animate({
        draw: (tf) => {
          let value = `rgb(${255 - tf * 255},${tf * 255},0)`;
          blocks[step.first].setAttribute("fill", value);
          blocks[step.second].setAttribute("fill", value);
        },
        duration: 300
      });
      await sleep(100);

      await animate({
        draw: (tf) => {
          let value = `rgb(${255 - tf * 255},${tf * 255},0)`;
          blocks[step.first].transform.baseVal[0].setTranslate(elementSize * (step.first + (step.second - step.first) * tf), 0);
          blocks[step.second].transform.baseVal[0].setTranslate(elementSize * (step.second + (step.first - step.second) * tf), 0);
        },
        duration: 400 / speed
      });
      await sleep(400 / speed);

      await animate({
        draw: (tf) => {
          let value = `rgb(${tf * 255},${255-tf * 255},0)`;
          blocks[step.first].setAttribute("fill", value);
          blocks[step.second].setAttribute("fill", value);
        },
        duration: 300
      });

      swap(blocks, step.first, step.second);
    }
    else if(step.type == SortStepTypes.BOUND) {
      bounds[0].setAttribute("x", elementSize * step.first - elementSize / 4);
      bounds[1].setAttribute("x", elementSize * (step.second + 1) - elementSize / 4);
    }

    await sleep(400 / speed);
  };
}


function drawElement(index, value) {
  let element = document.createElementNS("http://www.w3.org/2000/svg", "g");
  element.classList.add("element");

  let transform = svg.createSVGTransform();
  transform.setTranslate(elementSize * index,0);

  element.transform.baseVal.appendItem(transform)

  let rect = document.createElementNS("http://www.w3.org/2000/svg", "rect");
  rect.setAttribute("width", elementSize);
  rect.setAttribute("rx", 4);
  rect.setAttribute("ry", 4);
  rect.setAttribute("height", 5 * value);
  element.appendChild(rect);

  svg.appendChild(element);
  blocks.push(element);
}
function drawArray(array) {
  svg.innerHTML = null;
  blocks.length = 0;

  array.forEach((value, index) => {
    drawElement(index, value);
  });


  for(let i = 0;i < 2;++i) {
    let rect = document.createElementNS("http://www.w3.org/2000/svg", "rect");
    rect.classList.add("bound");
    rect.setAttribute("width", elementSize / 2);
    rect.setAttribute("rx", 4);
    rect.setAttribute("ry", 4);
    rect.setAttribute("height", 500);
    rect.setAttribute("fill", "black");

    bounds.push(rect);
    svg.appendChild(rect);
  }
}

// UI handlers
const arraySizeInput = document.getElementById('array-size');
let array;
function GenerateArray() {
  let size = Number(arraySizeInput.value) || 100;
  elementSize = (boardWidth + .0) / size;

  array = [];
  for(let i = 0;i < size;++i)
    array.push(randomInt(10, 100));

  drawArray(array);
}
GenerateArray();

const sortTypeInput = document.getElementById('sort-type');
const operationsCountLable = document.getElementById('operations-count');

function SortArray() {
  let type = sortTypeInput.value

  let result = SortsList.get(type).invoke(array);

  handleSteps(result, 10);
}
