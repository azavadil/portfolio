function post_to_url(selectedAction) {

    var form = document.createElement("form");
    form.setAttribute("method", "post");
	form.setAttribute("action", "#"); 
	
	var hiddenField1 = document.createElement("input"); 
    hiddenField1.setAttribute("type", "hidden");
	hiddenField1.setAttribute("name", "selectedAction");
    hiddenField1.setAttribute("value", selectedAction);  
    form.appendChild(hiddenField1); 
	
	var hiddenField2 = document.createElement("input"); 
    hiddenField2.setAttribute("type", "hidden");
	hiddenField2.setAttribute("name", "msgAuthor");
    hiddenField2.setAttribute("value", document.getElementById("msgAuthor").value);  
    form.appendChild(hiddenField2); 
	
	var hiddenField3 = document.createElement("input"); 
    hiddenField3.setAttribute("type", "hidden");
	hiddenField3.setAttribute("name", "msgSubject");
    hiddenField3.setAttribute("value", document.getElementById("msgSubject").firstChild.nodeValue);  
    form.appendChild(hiddenField3); 
	
	document.body.appendChild(form);
    form.submit();
}

function post_to_url_short(selectedAction) {

    var form = document.createElement("form");
    form.setAttribute("method", "post");
	form.setAttribute("action", "#"); 
	
	var hiddenField1 = document.createElement("input"); 
    hiddenField1.setAttribute("type", "hidden");
	hiddenField1.setAttribute("name", "selectedAction");
    hiddenField1.setAttribute("value", selectedAction);  
    form.appendChild(hiddenField1); 
	
	var hiddenField2 = document.getElementById("hiddenPageNum"); 
	form.appendChild(hiddenField2); 
	
	document.body.appendChild(form);
    form.submit();
}

function groupActionFn(selectedAction) {
	
    var form = document.createElement("form");
    form.setAttribute("method", "post");
    form.setAttribute("action", "#");
    
    var hiddenField1 = document.createElement("input"); 
    hiddenField1.setAttribute("type", "hidden");
	hiddenField1.setAttribute("name", "selectedAction");
    hiddenField1.setAttribute("value", selectedAction);  
    form.appendChild(hiddenField1); 
    
    var hiddenField2 = document.createElement("input"); 
    hiddenField2.setAttribute("type", "hidden");
	hiddenField2.setAttribute("name", "groupname");
    hiddenField2.setAttribute("value", document.getElementById("InputGroupname").value )    
    form.appendChild(hiddenField2);
    
    document.body.appendChild(form);
    form.submit();
}





function unusedFn(path, params, method) {
    method = method || "post"; // Set method to post by default if not specified.

    // The rest of this code assumes you are not using a library.
    // It can be made less wordy if you use one.
    var form = document.createElement("form");
    form.setAttribute("method", method);
    form.setAttribute("action", path);

    for(var key in params) {
        if(params.hasOwnProperty(key)) {
            var hiddenField = document.createElement("input");
            hiddenField.setAttribute("type", "hidden");
            hiddenField.setAttribute("name", key);
            hiddenField.setAttribute("value", params[key]);

            form.appendChild(hiddenField);
         }
    }

    document.body.appendChild(form);
    form.submit();
}