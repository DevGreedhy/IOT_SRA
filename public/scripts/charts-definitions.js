window.addEventListener('load', onload);

var chartS;

function onload(event) {
  chartS = createsensorChart();
  //listenToFirebaseUpdates();
}

// Función para crear el gráfico de humedad
function createsensorChart() {
  var chart = new Highcharts.Chart({
    chart: {
      renderTo: 'chart-sensor',
      type: 'spline'
    },
    series: [{
      name: 'Sensor'
    }],
    title: {
      text: undefined
    },
    plotOptions: {
      line: {
        animation: false,
        dataLabels: {
          enabled: true
        }
      },
      series: {
        color: '#50b8b4'
      }
    },
    xAxis: {
      type: 'datetime',
      labels: {
        formatter: function () {
          // Formatear la etiqueta del eje X con día y hora
          var date = new Date(this.value);
          var day = date.toLocaleDateString(); // Día en formato local
          var time = date.toLocaleTimeString(); // Hora en formato local
          return day + '<br>' + time;
        }
      }
    },
    yAxis: {
      title: {
        text: 'PORCENTAJE DE HUMEDAD (%)'
      }
    },
    credits: {
      enabled: false
    }
  });
  return chart;
}
/*
// Función para escuchar actualizaciones en Firebase
function listenToFirebaseUpdates() {
  var sensorRef = firebase.database().ref(databasePath + "/sensor");
  var bombaRef = firebase.database().ref(databasePath + "/bomba");

  // Escuchar cambios en el estado de la bomba
  bombaRef.on('value', function (snapshot) {
    var bomba = snapshot.val();
    updateBombaState(bomba);
  });

  // Escuchar cambios en el porcentaje de humedad
  sensorRef.on('value', function (snapshot) {
    var ValorSensor = snapshot.val();
    updatesensorChart(ValorSensor); // <-- Corregir el nombre de la función
  });
}

// Función para actualizar el gráfico de humedad
function updatesensorChart(value) {
  var time = new Date().getTime();
  chartS.series[0].addPoint([time, value]);
  updatesensorValue(value);
}

// Función para actualizar el valor de humedad en el HTML
function updatesensorValue(value) {
  var sensorValueElement = document.getElementById("sensor");
  sensorValueElement.textContent = value + " %";
}

// Función para actualizar el estado de la bomba en el HTML con color dinámico
function updateBombaState(state) {
  var bombaElement = document.getElementById("bomba");
  var bombaIcon = bombaElement.querySelector("i.fa");
  var bombaReading = bombaElement.querySelector(".reading");

  // Actualizar el texto y color del ícono según el estado
  if (state) {
    bombaReading.textContent = "Encendida";
    bombaIcon.style.color = "#19d600"; // Verde
  } else {
    bombaReading.textContent = "Apagada";
    bombaIcon.style.color = "#e74c3c"; // Rojo
  }
}
*/