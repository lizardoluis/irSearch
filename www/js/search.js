$(document).ready(function() {
	$(".dropdown-menu li a").click(function(){
		$("#model").find('.selection').text($(this).text());
		$("#model").find('.selection').val($(this).text());
	});

	$("#search").click(function(){	
		submit();
	});
	
	$("body").keypress(function(event) {
	    if (event.which == 13) {
	    	event.preventDefault();
	        submit();
	    }
	});
});

function submit() {
	var $model = $("#model").find('.selection').text();
	var $query = $("#query").val();

	if($query == "")
		return;

	$.get( "searcher.php", { model: $model, query: $query } )
	.done(function( msg ) {
		try {
			msg = msg.replace(/[^\x20-\x7E]+/g, '');
			
			console.log(msg);
			var obj = JSON.parse(msg);

			var size = obj.size;
			//console.log(size);

			var time = obj.time;
			var max = 10;

			if(size == 0){
				$("#stats_container > p").text("No results found");
				$("#result_container").html(""); 
				return;
			}
			else if(size == 1){
				$("#stats_container > p").text("1 result (" + time + " seconds)");
			}
			else if(size <= max){
				$("#stats_container > p").text(size + " results (" + time + " seconds)");
				max = size;
			}
			else {
				$("#stats_container > p").text("Showing top " + max + " results out of " + size + " found (" + time + " seconds)");
			}

			var html = "";
			for(var i=0; i<max; i++){
				var title = obj.data[i].title;
				var url = obj.data[i].url;
				html += "<div class='list-group'><h4 class='list-group-item-heading'><strong>" + title + "</strong></h4><p class='list-group-item-text'><a href='" + url + "'>" + url + "</a></p></div>";
			}
			$("#result_container").html(html); 

		} catch (e) {
			console.log(e.message);
			return showError();
		}
	})
	.fail(function() {
		return showError();
	});
}

function showError(){
	$("#stats_container > p").html("<strong>Oh sorry!</strong> Something isn't right!");
	$("#result_container").html("");
	return false;
}
