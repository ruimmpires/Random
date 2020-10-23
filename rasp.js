fetch('http://g0mesp1res.dynip.sapo.pt/rtemp.csv')
	.then(function (response) {
		return response.text();
	})
	.then(function (text) {
		let series = csvToSeries4(text);
		renderChart4(series);
		console.log(series);
	})
	.catch(function (error) {
		//Something went wrong
		console.log(error);
	});
	

function csvToSeries4(text) {
	const formatYmd = date => date.toISOString().slice(0, 10);
	let dataAsJson = JSC.csv2Json(text);
	let rtemp =[];
	dataAsJson.forEach(function (row) {
		
		rtemp.push({x: row.date, y: row.rtemp});
	});
	return [
		{name: 'Raspberry CPU', points: rtemp}
	];
}


function renderChart4(series) {
	JSC.Chart('chartDiv_raspTemp', {
		title_label_text: 'Raspberry CPU temperature',
		annotations: [{
			label_text: 'Source: Raspberry',
			position: 'bottom left'
		}],
		xAxis: {
			scale_type: "time",
			defaultTick_enabled: true
			},
		yAxis: {
			scale: {
				range: { min: 40, max: 70, padding: 0.1 }
			},
			markers: [{ value: 5 }],
		},
		legend_visible: false,
		xAxis_crosshair_enabled: true,
		yAxis_crosshair_enabled: true,
		defaultSeries_lastPoint_label_text: '<b>%seriesName</b>',	
		series: series
	});
}
