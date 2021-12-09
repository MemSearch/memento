"use strict"


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
            if (document.getElementById("tags-error")) {
                var err_txt = document.getElementById("tags-error");
                err_txt.parentElement.removeChild(err_txt);
            }
            openMemInfo();

            const radios = document.querySelectorAll('input[name="templ"]')
            var radTempl;
            for (const r of radios) {
                if(r.checked) {
                    radTempl = r.value;
                    break;
                }
            }
            var txtData = document.getElementById("tags-text").value;
            console.log(txtData);

            // var data = {
            //     url1: "../static/img/Superman.jpg",
            //     url2: "../static/img/thanus.jpg",
            //     url3: "../static/img/thanus.jpg",
            //     url4: "../static/img/Superman.jpg",
            //     url5: "../static/img/Superman.jpg",
            //     url6: "../static/img/thanus.jpg",
            //     url7: "../static/img/thanus.jpg",
            //     url8: "../static/img/Superman.jpg",
            //     url9: "../static/img/Superman.jpg",
            // };

            var data = {
                template: radTempl,
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
            console.log(urls);

            if (urls === null) {
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
                var urls_count = Object.keys(urls).length;
                var url_cnt = 0;
                div.innerHTML = "<h3 class=center>Memes that we were able to find:</h3>";
                var div_in = document.createElement('div');
                div_in.classList = "center";

                for (var url in urls) {
                    div_in.innerHTML += "<img id=" + url + " class=mem-pic src=" + urls[url] + ' alt="">'
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

// const sendInfo = () => {
//     const form = document.querySelector(".form_body")

//     form.addEventListener('submit', e => {
//         const formData = new FormData(form)
//         sendData("https://jsonplaceholder.typicode.com/posts", formData)
//             .then(() => {
//                 form.reset();
//             })
//             .catch((err) => {
//                 console.log(err);
//             })
//     });
// }


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