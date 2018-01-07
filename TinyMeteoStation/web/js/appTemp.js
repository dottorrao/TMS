$(document).ready(function(){
	$.ajax({
		url: "http://funnytech.atwebpages.com/tms/dataTemp.php",
		method: "GET",
		success: function(data) {
			console.log(data);
			var period = [];
			var temp = [];

			for(var i in data) {
				period.push(data[i].timestamp);
				temp.push(data[i].temperature);
			}

			var chartdata = {
				labels: period,
				datasets : [
					{
						label: 'Temperature',
						backgroundColor: 'rgba(232, 37, 63, 0.75)',
						borderColor: 'rgba(200, 200, 200, 0.75)',
						hoverBackgroundColor: 'rgba(200, 200, 200, 1)',
						hoverBorderColor: 'rgba(200, 200, 200, 1)',
						data:temp,
                                                fill:false,
					},     
				]
			};

			var ctx = $("#mycanvasTemperature");

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