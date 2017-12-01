#ifndef UI_HPP
#define UI_HPP

static inline std::string CssInject(const std::string &css) {
  std::string esc;
  const auto hex = "0123456789ABCDEF";
  for (auto c : css) {
    esc = esc + "\\x" + hex[((c >> 4) & 0xf)] + hex[c & 0xf];
  }
  std::string js = R"(
    (function(css) {
      var style = document.createElement('style');
      var head = document.head || document.getElementsByTagName('head')[0];
      style.setAttribute('type', 'text/css');
      if (style.styleSheet) {
        style.styleSheet.cssText = css;
      } else {
        style.appendChild(document.createTextNode(css));
      }
      head.appendChild(style);
    })(")" + esc + R"("))";
  return js;
}

static const char *html_data_uri =
    "data:text/"
    "html,%3C%21DOCTYPE%20html%3E%0A%3Chtml%20lang=%22en%22%3E%0A%3Chead%3E%"
    "3Cmeta%20charset=%22utf-8%22%3E%3Cmeta%20http-equiv=%22X-UA-Compatible%22%"
    "20content=%22IE=edge%22%3E%3C%2Fhead%3E%0A%3Cbody%3E%3Cdiv%20id=%22app%22%"
    "3E%3C%2Fdiv%3E%3Cscript%20type=%22text%2Fjavascript%22%3E%3C%2Fscript%3E%"
    "3C%2Fbody%3E%0A%3C%2Fhtml%3E";

static const char *styles_css = R"stylescss(
* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}
body, html, .container { height: 100%; overflow: hidden; }

.editor {
    margin: 0;
    padding: 1em;
    font-family: monospace;
    width: 100%;
    height: 100%;
    resize: none;
    border: none;
    outline: none;
    overflow: auto;
    background-color: #ffffff;
}

.icon-bar {
    position: fixed;
    top: 0;
    right: 0;
    margin: 1em;
    height: 32px;
    border-radius: 16px;
    background-color: #fff;
}

.icon {
    display: inline-block;
    width: 32px;
    height: 32px;
    cursor: pointer;
    border-radius: 50%;
    background-repeat: no-repeat;
    background-position: center;
}
.icon:not(:first-child) {
    margin-left: 0.5em;
}
.icon-create-file {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23cccccc' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M19 13h-6v6h-2v-6H5v-2h6V5h2v6h6v2z'/%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3C/svg%3E");
}
.icon-open-file {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23cccccc' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M20 6h-8l-2-2H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2zm0 12H4V8h16v10z'/%3E%3C/svg%3E");
}
.icon-export-pdf {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23cccccc' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M5 4v2h14V4H5zm0 10h4v6h6v-6h4l-7-7-7 7z'/%3E%3C/svg%3E");
}
.icon-settings {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23cccccc' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M12 3c-4.97 0-9 4.03-9 9s4.03 9 9 9c.83 0 1.5-.67 1.5-1.5 0-.39-.15-.74-.39-1.01-.23-.26-.38-.61-.38-.99 0-.83.67-1.5 1.5-1.5H16c2.76 0 5-2.24 5-5 0-4.42-4.03-8-9-8zm-5.5 9c-.83 0-1.5-.67-1.5-1.5S5.67 9 6.5 9 8 9.67 8 10.5 7.33 12 6.5 12zm3-4C8.67 8 8 7.33 8 6.5S8.67 5 9.5 5s1.5.67 1.5 1.5S10.33 8 9.5 8zm5 0c-.83 0-1.5-.67-1.5-1.5S13.67 5 14.5 5s1.5.67 1.5 1.5S15.33 8 14.5 8zm3 4c-.83 0-1.5-.67-1.5-1.5S16.67 9 17.5 9s1.5.67 1.5 1.5-.67 1.5-1.5 1.5z'/%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3C/svg%3E");
}
.icon-create-file:hover {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23444444' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M19 13h-6v6h-2v-6H5v-2h6V5h2v6h6v2z'/%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3C/svg%3E");
}
.icon-open-file:hover {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23444444' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M20 6h-8l-2-2H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2zm0 12H4V8h16v10z'/%3E%3C/svg%3E");
}
.icon-export-pdf:hover {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23444444' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M5 4v2h14V4H5zm0 10h4v6h6v-6h4l-7-7-7 7z'/%3E%3C/svg%3E");
}
.icon-settings:hover {
    background-image: url("data:image/svg+xml,%3Csvg fill='%23444444' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M12 3c-4.97 0-9 4.03-9 9s4.03 9 9 9c.83 0 1.5-.67 1.5-1.5 0-.39-.15-.74-.39-1.01-.23-.26-.38-.61-.38-.99 0-.83.67-1.5 1.5-1.5H16c2.76 0 5-2.24 5-5 0-4.42-4.03-8-9-8zm-5.5 9c-.83 0-1.5-.67-1.5-1.5S5.67 9 6.5 9 8 9.67 8 10.5 7.33 12 6.5 12zm3-4C8.67 8 8 7.33 8 6.5S8.67 5 9.5 5s1.5.67 1.5 1.5S10.33 8 9.5 8zm5 0c-.83 0-1.5-.67-1.5-1.5S13.67 5 14.5 5s1.5.67 1.5 1.5S15.33 8 14.5 8zm3 4c-.83 0-1.5-.67-1.5-1.5S16.67 9 17.5 9s1.5.67 1.5 1.5-.67 1.5-1.5 1.5z'/%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3C/svg%3E");
}
)stylescss";

static const char *picodom_js = R"picodomjs(
!function(e,r){"object"==typeof exports&&"undefined"!=typeof module?r(exports):"function"==typeof define&&define.amd?define(["exports"],r):r(e.picodom={})}(this,function(e){"use strict";function r(e,r){var n,t=[];for(s=arguments.length;s-- >2;)c.push(arguments[s]);for(;c.length;)if(Array.isArray(n=c.pop()))for(s=n.length;s--;)c.push(n[s]);else null!=n&&!0!==n&&!1!==n&&t.push("number"==typeof n?n+="":n);return"string"==typeof e?{type:e,props:r||{},children:t}:e(r||{},t)}function n(e,r,n,t){for(var o=l(n||(n=document.body),n.children[0],e,r);t=a.pop();)t();return o}function t(e,r){var n={};for(var t in e)n[t]=e[t];for(var t in r)n[t]=r[t];return n}function o(e,r){if("string"==typeof e)var n=document.createTextNode(e);else{var n=(r=r||"svg"===e.type)?document.createElementNS("http://www.w3.org/2000/svg",e.type):document.createElement(e.type);e.props&&e.props.oncreate&&a.push(function(){e.props.oncreate(n)});for(var t=0;t<e.children.length;t++)n.appendChild(o(e.children[t],r));for(var t in e.props)p(n,t,e.props[t])}return n}function p(e,r,n,o){if("key"===r);else if("style"===r)for(var r in t(o,n=n||{}))e.style[r]=n[r]||"";else{try{e[r]=n}catch(e){}"function"!=typeof n&&(n?e.setAttribute(r,n):e.removeAttribute(r))}}function i(e,r,n){for(var o in t(r,n)){var i=n[o],f="value"===o||"checked"===o?e[o]:r[o];i!==f&&p(e,o,i,f)}n&&n.onupdate&&a.push(function(){n.onupdate(e,r)})}function f(e,r,n){function t(){e.removeChild(r)}n&&n.onremove&&"function"==typeof(n=n.onremove(r))?n(t):t()}function u(e){if(e&&e.props)return e.props.key}function l(e,r,n,t,p,s){if(null==n)r=e.insertBefore(o(t,p),r);else if(null!=t.type&&t.type===n.type){i(r,n.props,t.props),p=p||"svg"===t.type;for(var c=t.children.length,a=n.children.length,d={},h=[],v={},y=0;y<a;y++){var g=h[y]=r.childNodes[y],m=n.children[y],b=u(m);null!=b&&(d[b]=[g,m])}for(var y=0,k=0;k<c;){var g=h[y],m=n.children[y],w=t.children[k],b=u(m);if(v[b])y++;else{var x=u(w),A=d[x]||[];null==x?(null==b&&(l(r,g,m,w,p),k++),y++):(b===x?(l(r,A[0],A[1],w,p),y++):A[0]?(r.insertBefore(A[0],g),l(r,A[0],A[1],w,p)):l(r,g,null,w,p),k++,v[x]=w)}}for(;y<a;){var m=n.children[y],b=u(m);null==b&&f(r,h[y],m.props),y++}for(var y in d){var A=d[y],B=A[1];v[B.props.key]||f(r,A[0],B.props)}}else r&&t!==r.nodeValue&&("string"==typeof t&&"string"==typeof n?r.nodeValue=t:(r=e.insertBefore(o(t,p),s=r),f(e,s,n.props)));return r}var s,c=[],a=[];e.h=r,e.patch=n});
)picodomjs";

static const char *app_js = R"appjs(

var PALETTE = [
    {fg: 0xfffefefe, bg: 0xff333333},    // white on dark gray
    {fg: 0xff333333, bg: 0xffeeeeee},    // dark gray on white
    {fg: 0xffffffff, bg: 0xffe51c23},    // white on red
    {fg: 0xffffffff, bg: 0xffff5722},    // white on deep orange
    {fg: 0xff222222, bg: 0xffffeb3b},    // black on yellow
    {fg: 0xff333333, bg: 0xffcddc39},    // black on lime
    {fg: 0xff222222, bg: 0xff8bc34a},    // black on light green
    {fg: 0xffffffff, bg: 0xff009688},    // white on teal
    {fg: 0xff333333, bg: 0xff00bcd4},    // black on cyan
    {fg: 0xffffffff, bg: 0xff3f51b5},    // white on indigo
    {fg: 0xffffffff, bg: 0xff75507b},    // white on plum
    {fg: 0xffffffff, bg: 0xffe91e63},    // white on pink
];

window.app = {
    cmd: function(c) { window.external.invoke_(JSON.stringify(c)); },
    setCursor: function(pos) { app.cmd({cmd: 'set_cursor', cursor: pos}); },
    setText: function(s) { app.cmd({cmd: 'set_text', text: s}); },
    setPalette: function(fg, bg) { app.cmd({cmd: 'set_palette', fg: fg, bg: bg}); },
    setPreviewSize: function(w, h) { app.cmd({cmd: 'set_preview_size', w: w, h: h}); },
    createFile: function() { app.cmd({cmd: 'create_file'}); },
    openFile: function() { app.cmd({cmd: 'open_file'}); },
    exportPDF: function() { app.cmd({cmd: 'export_pdf'}); },
    currentPalette: 0,
    state: {
        text: '',
        previewDataURI: '',
    },
};

function ui() {
    var h = picodom.h;
    return h('div', { class: 'container' },
            h('textarea', {
                class: 'editor',
                autofocus: true,
                onclick: function() { app.setCursor(this.selectionStart) },
                onkeyup: function() { app.setCursor(this.selectionStart) },
                oninput: function() {
                    app.setText(this.value);
                    app.setCursor(this.selectionStart);
                },
                onupdate: function(el, attr) {

                },
                value: app.state.text,
            }),
            h('div', {class: 'icon-bar'}, 
                h('div', {
                        class: 'icon icon-create-file',
                        title: 'Create new (Ctrl+N)',
                        onclick: app.createFile,
                }),
                h('div', {
                        class: 'icon icon-open-file',
                        title: 'Open file (Ctrl+O)',
                        onclick: app.openFile,
                }),
                h('div', {
                        class: 'icon icon-export-pdf',
                        title: 'Export to PDF (Ctrl+P)',
                        onclick: app.exportPDF,
                }),
                h('div', {
                        class: 'icon icon-settings',
                        title: 'Change palette (Ctrl+, / Ctrl+.)',
                        onclick: function() {
                            app.currentPalette =
                                (app.currentPalette + 1) % PALETTE.length;
                            app.setPalette(PALETTE[app.currentPalette].fg,
                                PALETTE[app.currentPalette].bg);
                        },
                })),
            h('img', {
                onupdate: function(el, attr) {
                },
                src: app.state.previewDataURI,
                style: {
                    position: 'fixed',
                    width: '200px',
                    height: '150px',
                    bottom: '1em',
                    right: '1em',
                    boxShadow: '0 5px 6px 0 rgba(0,0,0,0.2)',
                },
            }));
}

var node;
window.render = function() {
    picodom.patch(node, (node = ui()), document.body);
}

document.onkeydown = function(e) {
    if (e.keyCode === 78 && e.ctrlKey) {  // Ctrl+N
        e.preventDefault();
        app.createFile();
    } else if (e.keyCode === 79 && e.ctrlKey) {  // Ctrl+O
        e.preventDefault();
        app.openFile();
    } else if (e.keyCode === 80 && e.ctrlKey) {  // Ctrl+P
        e.preventDefault();
        app.exportPDF();
    } else if (e.keyCode === 188 && e.ctrlKey) {  // Ctrl+,
        app.currentPalette = (app.currentPalette - 1 + PALETTE.length) % PALETTE.length;
        app.setPalette(PALETTE[app.currentPalette].fg, PALETTE[app.currentPalette].bg);
    } else if (e.keyCode === 190 && e.ctrlKey) {  // Ctrl+.
        app.currentPalette = (app.currentPalette + 1) % PALETTE.length;
        app.setPalette(PALETTE[app.currentPalette].fg, PALETTE[app.currentPalette].bg);
    }
};

app.setPalette(PALETTE[app.currentPalette].fg, PALETTE[app.currentPalette].bg);
app.setPreviewSize(200, 150);
render();
)appjs";

#endif // UI_HPP
