const dados = require('../controllers/dados');

exports.build = function(server){
  var api = '/api';
  server
    .get(api  + '/dados',       dados.getall)
    .post(api + '/dados',       dados.add)

}
