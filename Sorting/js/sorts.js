SortStepTypes = {
  COMPARE: "compare", // compare elements
  SWAP: "swap", // swap elements
  SELECT: "select", // select 1 element
  BOUND: "bound" // sorting bounds
}

class Sort {
  constructor(name, description, invoke) {
    this.name = name;
    this.description = description;

    this.invoke = invoke;
  }
}

SortsList = {
  dict: {},
  add: function (sort) {
    if(sort.name) {
      if(this.dict[sort.name])
        console.warn(`Sort ${sort.name} is overrided`, sort);

      this.dict[sort.name] = sort;
    }
    else
      console.error("Invalid sort", sort);
  },
  get: function(name) {
    return this.dict[name];
  },
  getAll: function () {
    return Object.values(this. dict);
  }
}

SortsList.add(
  new Sort(
    "BubbleSort",
    "---",
    function(data) {
      let result = [];

      let lastChange = result.length;
      while(lastChange > 1) {
        let curLastChange = -1;

        result.push({
          type: SortStepTypes.BOUND,
          first: 0,
          second: lastChange
        });

        for(let j = 0;j < lastChange - 1;++j) {
          if(data[j] > data[j + 1])
          {
            result.push({
              type: SortStepTypes.SWAP,
              first: j,
              second: j+1
            });

            curLastChange = j+1;
            swap(data, j, j+1);
          }
          else {
            result.push({
              type: SortStepTypes.COMPARE,
              first: j,
              second: j+1
            });
          }

          lastChange = curLastChange;
        }
      }

      return result;
    }
  )
);
SortsList.add(
  new Sort(
    "Quicksort",
    "---",
    function(data) {
      let result = [];

      function sort(from, to) {
        result.push({
          type: SortStepTypes.BOUND,
          first: from,
          second: to
        });

        let et = data[to];

        let m = from;
        for(let i = from;i < to;++i) {
          if(data[i] < et) {
            if(i != m) {
              result.push({
                type: SortStepTypes.SWAP,
                first: i,
                second: m
              });

              swap(data, i, m);
            }
            m++;
          }
          else {
            if(i != m)
              result.push({
                type: SortStepTypes.COMPARE,
                first: i,
                second: m
              });
          }
        }
        if(m != to) {
            result.push({
              type: SortStepTypes.SWAP,
              first: m,
              second: to
            });

            swap(data, m, to)
        }

        if(from < m-1)
          sort(from, m-1);
        if(m+1 < to)
          sort(m+1, to);
      }

      sort(0, data.length-1);
      return result;
    }
  )
);
SortsList.add(
  new Sort(
    "SelectionSort",
    "---",
    function (data) {
      let result = [];

      for(let i = 0;i < data.length;++i) {
        let et = i;
        for(let j = i+1;j < data.length;++j) {
          result.push({
            type: SortStepTypes.COMPARE,
            first: j,
            second: et
          });

          if(data[j] < data[et])
            et = j;
        }

        if(et != i) {
          result.push({
            type: SortStepTypes.SWAP,
            first: i,
            second: et
          });
          swap(data, et, i);
        }
      }

      return result;
    }
  )
);
SortsList.add(
  new Sort(
    "InsertionSort",
    "---",
    function (data) {
      let result = [];

      for(let i = 0;i < data.length;++i) {
        for(let j = i-1;j >= 0 && data[j] > data[j+1];--j) {
          result.push({
            type: SortStepTypes.COMPARE,
            first: j,
            second: j+1
          });
        }
      }

      return result;
    }
  )
);
