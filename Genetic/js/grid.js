$("[data-copy]").get().reverse().forEach(function(val) {
	var item = $(val);
	
	var cnt = item.data("copy");
	if(isNaN(cnt))
		return;
	
	cnt = Number(cnt);
	item.removeAttr("data-copy");
	
	for(var i = 1;i < cnt;++i)
		item.after(item.clone());
});