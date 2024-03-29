//fetch('http://g0mesp1res.dynip.sapo.pt/mqttstats.csv')
fetch('../mqttstats.csv')
        .then(function (response) {
                return response.text();
        })
        .then(function (text) {
                //outdoor temperature
                let series = csvToSeries(text);
                renderChart(series);
                console.log(series);
                //outdoor humidity
                let series2 = csvToSeries2(text);
                renderChart2(series2);
                console.log(series2);
                //water temperature
                let series3 = csvToSeries3(text);
                renderChart3(series3);
                console.log(series3);
                //power
                let series5 = csvToSeries5(text);
                renderChart5(series5);
                console.log(series5);
                //potential
                let series6 = csvToSeries6(text);
                renderChart6(series6);
                console.log(series6);

        })
        .catch(function (error) {
                //Something went wrong
                //console.log(error);
        });

function csvToSeries(text) {
        const formatYmd = date => date.toISOString().slice(0, 10);
        let dataAsJson = JSC.csv2Json(text);
        let outtemp =[];
        dataAsJson.forEach(function (row) {
                outtemp.push({x: row.date, y: row.outtemp});
        });
        return [
                {name: 'Outdoor temperature', points: outtemp}
        ];
}

function csvToSeries2(text) {
        const formatYmd = date => date.toISOString().slice(0, 10);
        let dataAsJson = JSC.csv2Json(text);
        let hum =[];
        dataAsJson.forEach(function (row) {
                hum.push({x: row.date, y: row.hum});
        });
        return [
                {name: 'Outdoor Humidity', points: hum}
        ];
}

function csvToSeries3(text) {
        const formatYmd = date => date.toISOString().slice(0, 10);
        let dataAsJson = JSC.csv2Json(text);
        let wtemp =[];
        dataAsJson.forEach(function (row) {
                wtemp.push({x: row.date, y: row.wtemp});
        });
        return [
                {name: 'Water temperature', points: wtemp}
        ];
}

function csvToSeries5(text) {
        const formatYmd = date => date.toISOString().slice(0, 10);
        let dataAsJson = JSC.csv2Json(text);
        let power =[];
        dataAsJson.forEach(function (row) {
                power.push({x: row.date, y: row.power});
        });
        return [
                {name: 'power', points: power}
        ];
}

function csvToSeries6(text) {
        const formatYmd = date => date.toISOString().slice(0, 10);
        let dataAsJson = JSC.csv2Json(text);
        let potential =[];
        dataAsJson.forEach(function (row) {
                potential.push({x: row.date, y: row.potential});
        });
        return [
                {name: 'potential', points: potential}
        ];
}


function renderChart(series) {
        JSC.Chart('chartDiv_outdoorTemp', {
                title_label_text: 'Outdoor temperature',
                annotations: [{
                        label_text: 'Source: Raspberry and MQTT',
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

function renderChart2(series) {
        JSC.Chart('chartDiv_outdoorHum', {
                title_label_text: 'Outdoor humidity',
                annotations: [{
                        label_text: 'Source: Raspberry and MQTT',
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

function renderChart3(series) {
        JSC.Chart('chartDiv_waterTemp', {
                title_label_text: 'Water temperature',
                annotations: [{
                        label_text: 'Source: Raspberry and MQTT',
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

function renderChart5(series) {
        JSC.Chart('chartDiv_power', {
                title_label_text: 'Power in VA',
                annotations: [{
                        label_text: 'Source: Raspberry and MQTT',
                        position: 'bottom left'
                }],
                xAxis: {
                        scale_type: "time",
                        defaultTick_enabled: true
                        },
                yAxis: {
                        scale: {
                                range: { min: 0, max: 7000, padding: 0.1 }
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

function renderChart6(series) {
        JSC.Chart('chartDiv_potential', {
                title_label_text: 'Potential in V',
                annotations: [{
                        label_text: 'Source: Raspberry and MQTT',
                        position: 'bottom left'
                }],
                xAxis: {
                        scale_type: "time",
                        defaultTick_enabled: true
                        },
                yAxis: {
                        scale: {
                                range: { min: 200, max: 250, padding: 0.1 }
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
