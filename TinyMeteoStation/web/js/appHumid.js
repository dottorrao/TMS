$(document).ready(function(){
	$.ajax({
		url: "http://funnytech.atwebpages.com/tms/dataTemp.php",
		method: "GET",
		success: function(data) {
			console.log(data);
			var period = [];
			var hum = [];
 
			for(var i in data) {
				period.push(data[i].timestamp);
				hum.push(data[i].humidity);
			}
 
			var chartdata = {
				labels: period,
				datasets : [
					{
						label: 'Humidity',
						backgroundColor: 'rgba(0, 137, 255, 0.75)',
						borderColor: 'rgba(200, 200, 200, 0.75)',
						hoverBackgroundColor: 'rgba(200, 200, 200, 1)',
						hoverBorderColor: 'rgba(200, 200, 200, 1)',
						data:hum,
                                                fill:false,
					},     
				]
			};
 
			var ctx = $("#mycanvasHumidity");
 
			var barGraph = new Chart(ctx, {
				type: 'line',
				data: chartdata
			});
		},
		error: function(data) {
			console.log(data);
                        alert(data);
		}
	});
});
