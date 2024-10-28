function updateList(dados){
	$("#listTable tbody").empty()
	var table_html = ''

	dados.forEach(function(item, index){
		table_html += "<tr>"
		table_html += "<th>" + item.Humidade + "</th>"
		table_html += "<th>" + item.Temperatura + "</th>"
		table_html += "<th>" + item.Date + "</th>"
		table_html += "</tr>"
	})
	$("#listTable tbody").html(table_html);
}


$(document).ready(function(){
	$.ajax({
		method: "GET",
		url: "/api/dados",
	}).done(function(resp){
		updateList(resp);
	})
	.fail(function() {
    	alert( "error" );
  	})
})
