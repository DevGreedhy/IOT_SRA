
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");

// Elements for sensor readings
const sensorElement = document.getElementById("sensor");
const bombaElement = document.getElementById("bomba");

var dbPathBomba;
var dbRefSensor;  // Declare dbRefSensor outside the setupUI function


// MANAGE LOGIN/LOGOUT UI
const setupUI = (user) => {
  if (user) {
    //toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display ='block';
    userDetailsElement.style.display ='block';
    userDetailsElement.innerHTML = user.email;

    // get user UID to get data from the database
    var uid = user.uid;
    console.log(uid);

    // Database paths (with user UID)
    var dbPathSensor = 'UsersData/' + uid.toString() + '/sensor';
    dbPathBomba = 'UsersData/' + uid.toString() + '/bomba';

    // Database references
    dbRefSensor = firebase.database().ref().child(dbPathSensor);  // Initialize dbRefSensor

    var dbRefBomba = firebase.database().ref().child(dbPathBomba);

    // Update page with new readings
    dbRefSensor.on('value', snap => {
      sensorElement.innerText = snap.val();
    });

    dbRefBomba.on('value', snap => {
      bombaElement.innerText = snap.val();
    });

    // Update chart with new sensor readings
    dbRefSensor.on('value', snap => {
      sensorElement.innerText = snap.val().toFixed(2);
      var x = (new Date()).getTime();
      var y = parseFloat(snap.val().toFixed(2));

      if (chartS.series[0].data.length > 40) {
        chartS.series[0].addPoint([x, y], true, true, true);
      } else {
        chartS.series[0].addPoint([x, y], true, false, true);
      }
    });

    // Rest of the code... (additional adjustments may be needed based on the rest of the code)
  } else {
    // toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display ='none';
    userDetailsElement.style.display ='none';
    contentElement.style.display = 'none';
  }
}


// Obtener el botón y agregar un manejador de eventos al hacer clic
const generatePdfButton = document.getElementById('generate-pdf-button');
generatePdfButton.addEventListener('click', () => {
  // Lógica para generar el PDF
  console.log("no funciona");
  generatePDF();
});
// Función para formatear una hora a un formato legible (puedes personalizar esto según tus necesidades)
function formatHour(hour) {
  const options = { hour: 'numeric', minute: 'numeric', second: 'numeric' };
  return new Date(hour).toLocaleTimeString('es-ES', options);
}
function formatDay(timestamp) {
  const date = new Date(timestamp);
  return Highcharts.dateFormat('%e %b', date);
}

// Función para generar el PDF
function generatePDF() {
  // Utiliza la biblioteca jsPDF para generar el PDF
  console.log(window);
  console.log(jspdf);
  const pdf = new jspdf.jsPDF();
  console.log("nola");
  // Agrega contenido al PDF
  pdf.text('Reporte de Información', 20, 10);
  pdf.text(`Estado de la Bomba: ${bombaElement.innerText}`, 20, 20);
  pdf.text(`Humedad de la Planta: ${sensorElement.innerText}`, 20, 30);
  // Agrega datos del gráfico de humedad al PDF
  pdf.text('Datos del Gráfico de Humedad:', 20, 40);

  const chartData = chartS.series[0].data;

  chartData.forEach((point, index) => {
    const formattedHour = formatHour(point.x);
    const formattedDay = formatDay(point.x);
    pdf.text(`Día ${formattedDay}: Hora=${formattedHour}, Humedad=${point.y}`, 20, 50 + index * 10);
  });

  // Guarda el PDF (puedes ajustar el nombre del archivo según tus necesidades)
  console.log("puedo");
  pdf.save('reporte.pdf');
  console.log("no puedo");
}

