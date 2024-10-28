
const mysql = require('mysql');

var connection = mysql.createPool({
  connectionLimit : 10,
  host     : 'localhost',
  user     : 'farmer',
  password : 'qwer1234'
});

function getall(req,res){
	console.log("aqui");
        connection.query('SELECT * FROM farm_station.station_1', function(error, results){
                if(error){
                        res.sendStatus(500);
                } else {
			console.log("aqui");
                        res.send(results);
                }
        })
}

function add(req,res){
        let values = '"' + req.body.MAC + '","' + req.body.Humidade + '","' + req.body.Temperatura + '","' + req.body.Date + '"';
        let query = 'INSERT INTO farm_station.station_1(MAC,Humidade,Temperatura,Date) VALUES (' + values + ')';
         console.log(connection);
         console.log(query);
        connection.query(query, function(error, results){
                if (error){
                        console.log(error);
                        res.sendStatus(500);
                } else {
                        res.send(req.body);
                }
         })
}

module.exports = {getall, add}
