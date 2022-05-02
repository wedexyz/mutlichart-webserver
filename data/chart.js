
	//chart line
	var values = [];
	var timeStamp = [];
	function showGraph()
	{
		for (i = 0; i < arguments.length; i++) {
			values.push(arguments[i]);    
		}
	
		var ctx = document.getElementById("Chart").getContext('2d');
		var Chart2 = new Chart(ctx, {
			type: 'line',
			data: {
				labels: timeStamp,	
				datasets: [{
					label: "Voltage 1",
					fill: false,	//Try with true
					backgroundColor: '#2cc2ca', 
					borderColor: '#2cc2ca',	
					data: values,
				}],
			},
			options: {
				title: {
						display: true,
						text: "ADC Voltage"
					},
				maintainAspectRatio: false,
				elements: {
				line: {
						tension: 0.5 //Smoothening (Curved) of data lines
					}
				},
				scales: {
						yAxes: [{
							ticks: {
								beginAtZero:true
							}
						}]
				}
			}
		});
	
	}
	window.onload = function() {
		console.log(new Date().toLocaleTimeString());
		showGraph(5,10,4,58);
	};
	
	
	setInterval(function() {
	  getData();
	}, 1000); //1000mSeconds update rate
	 

	function getData() {
	  var xhttp = new XMLHttpRequest();
	  xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
		 //Push the data in array
		var time = new Date().toLocaleTimeString();
		var ADCValue = this.responseText; 
		  values.push(ADCValue);
		  timeStamp.push(time);
		  showGraph();	//Update Graphs
		//Update Data Table
		  var table = document.getElementById("dataTable");
		  var row = table.insertRow(1);	//Add after headings
		  var cell1 = row.insertCell(0);
		  var cell2 = row.insertCell(1);
		  cell1.innerHTML = time;
		  cell2.innerHTML = ADCValue;
		}
	  };
	  xhttp.open("GET", "readADC", false);	//Handle readADC server on ESP8266
	  xhttp.send();
	}




