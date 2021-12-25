"use strict"

function downloadAllChecked() {
    var meme_templates = document.getElementById("mem-info").querySelectorAll(".meme-template");
    const urls = [];
    for (let index = 0; index < meme_templates.length; index++) {
        if (meme_templates[index].checked) {
            var pic = meme_templates[index].parentElement.querySelectorAll(".mem-pic");
            var pic_src = pic[0].src;
            // var pic_webp = pic_src.substr(0, pic_src.lastIndexOf('.')) + ".JPEG";
            // console.log(pic_webp);
            // console.log(pic[0].src);
            urls.push(pic_src);
        }
    }
    console.log(urls);
    downloadAll(urls);
};

function downloadAll(urls) {
    var link = document.createElement('a');

    link.setAttribute('download', null);
    link.style.display = 'none';

    document.body.appendChild(link);

    for (var i = 0; i < urls.length; i++) {
        link.setAttribute('href', urls[i]);
        link.click();
    }

    document.body.removeChild(link);
}

function selectAllCheckBox() {
    var meme_templates = document.getElementById("mem-info").querySelectorAll(".meme-template");
    selectAll(meme_templates);
};

function selectAll(some_class) {
    console.log(some_class);
    for (let index = 0; index < some_class.length; index++) {
        if (!some_class[index].checked) {
            some_class[index].checked = true;
        }
    }
};

function viewDiv() {
    document.getElementById("mem-search").style.display = "block";
    document.getElementById("hello-text").style.display = "none";
};

function clearInfo(element) {
    element.innerHTML = "";
};

function openMemInfo() {
    clearInfo(document.getElementById("mem-info"));
    document.getElementById("mem-info").style.display = "block";
};

document.addEventListener('DOMContentLoaded', function () {
    const form = document.getElementById("form");
    form.addEventListener('submit', strSend);

    async function strSend(e) {
        e.preventDefault();
        let error = formValidate(form)
    }

    async function formValidate(form){
        let error = 0;
        let formReq = document.querySelectorAll("._req")

        for (let index = 0; index < formReq.length; index++) {
            const input = formReq[index];
            formRemoveError(input);

            if(input.value === "") {
                formAddError(input);
                error++;
                if (!document.getElementById("tags-error")) {
                    var err_txt = document.createElement('div');
                    err_txt.classList = "error-info";
                    err_txt.innerHTML = "<h6>This field is mandatory</h6>";
                    err_txt.id = "tags-error";
                    input.parentElement.insertAdjacentElement("afterbegin", err_txt);
                }
            }
        }

        if (error === 0) {
            console.log(document.querySelectorAll(".center"));
            if (document.getElementById("tags-error")) {
                var err_txt = document.getElementById("tags-error");
                err_txt.parentElement.removeChild(err_txt);
            }
            openMemInfo();

            var txtData = document.getElementById("tags-text").value;
            console.log(txtData);

            var data = {
                text: txtData,
            };

            let headers;
            let HTTPstatus;
            var my_data = await fetch("http://127.0.0.1:5000/", {
                method: 'POST',
                body: JSON.stringify(data),
            }).then( async (response) => {
                HTTPstatus = response.ok;
                headers = response.headers;
                console.log(data);
                return await response.json();
            }).then( async (data) => {
                console.log(data);
                return await data;
            });

            var json = JSON.stringify(my_data);
            var urls = JSON.parse(json);

            var div = document.createElement('div');
            div.id = "mem-pics";
            document.getElementById('mem-info').appendChild(div);
            var mem_btn_div = document.getElementById('mem-btn');
            console.log(urls);
            
            if (urls === null) {
                // mem_btn_div.style.display = "none";
                div.innerHTML = "<h4 class=center>Sorry, we couldn't find anything for your query.</h4>";
                div.innerHTML += "<h5 class=center>You can repeat the request or try to find the meme you are interested in yourself.</h5>"; 

                var div_in = document.createElement('div');
                div_in.classList = "center";

                div_in.innerHTML += "<h5>You can also help us and send a meme that you couldn't find with us</h5>";
                
                var label = "<label for=file-upload class=custom-file-upload accept='.jpg, .jpeg, .png'>upload</label>"
                div_in.innerHTML += label;

                var input = document.createElement('input');
                input.type = "file";
                input.accept = ".jpg, .jpeg, .png";
                input.id = "file-upload";
                div_in.appendChild(input);

                var button = document.createElement('button');
                button.innerHTML = "send";
                button.classList = "search-button";
                div_in.appendChild(button);

                div.appendChild(div_in);
            }
            else {
                div.innerHTML = "<h3 class=center>Memes that we were able to find:</h3>";
                var urls_count = Object.keys(urls).length
                var url_cnt = 0;
                var div_in = document.createElement('div');
                div_in.classList = "center";
                for (var url in urls) {
                    div_in.innerHTML += "<label><input type='checkbox' class='meme-template' name='templ'>\
                    <img id=" + url + " class=mem-pic src=" + urls[url] + "></label>";
                    if (url_cnt % 2 === 1) {
                        document.getElementById('mem-pics').appendChild(div_in);
                        div_in = document.createElement('div');
                        div_in.classList = "center";
                    }
                    url_cnt += 1;
                    if (url_cnt === urls_count) {
                        document.getElementById('mem-pics').appendChild(div_in);
                    }
                }
                // mem_btn_div.style.display = "flex";
                var div_upload = document.createElement('div');
                div_upload.classList = "center up-margin";
                // var select_all_btn = document.createElement('button');
                // select_all_btn.textContent = "select all";
                // select_all_btn.classList = "search-button left-margin";
                // select_all_btn.onclick = function() {
                //     alert("SAS");
                // }
                // div_upload.appendChild(select_all_btn);
                // <button class="search-button left-margin" onclick="selectAllCheckBox()">select all</button>\
                div_upload.innerHTML += '<button class="search-button left-margin" onclick="selectAllCheckBox()">select all</button>\
                                         <h3 class="">Ready to upload?</h3>\
                                         <button class="search-button" onclick="downloadAllChecked()">upload</button>';
                // <a href="../static/img/thanus.jpg" download="">\</a>
                document.getElementById('mem-pics').appendChild(div_upload);
            }
        }

    }
    function formAddError(input) {
        input.parentElement.classList.add("_error");
        input.classList.add("_error");
    }
    
    function formRemoveError(input) {
        input.parentElement.classList.remove("._error");
        input.classList.remove("_error");
    }
});

const getData = async (url) => {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error(`Ошибка по адресу ${url}, статус ошибки ${response.status}`);
    }
    return await response.json();
};
    
const sendData = (url, data) => {
    const response = fetch(url, {
        method: 'GET',
        body: JSON.stringify(data),
    });
    if (!response.ok) {
        throw new Error(`Ошибка по адресу ${url}, статус ошибки ${response.status}`);
    }
    return response.json();
};

function test() {
    document.write("Test works fine");
};
