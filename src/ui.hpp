#ifndef UI_HPP
#define UI_HPP

static inline std::string css_inject(const std::string &css) {
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
    background-image: url("data:image/svg+xml,%3Csvg fill='%23cccccc' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M19.43 12.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.39-.3-.61-.22l-2.49 1c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46 2.18 14.25 2 14 2h-4c-.25 0-.46.18-.49.42l-.38 2.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49 0-.61.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11 1.65c-.19.15-.24.42-.12.64l2 3.46c.12.22.39.3.61.22l2.49-1c.52.4 1.08.73 1.69.98l.38 2.65c.03.24.24.42.49.42h4c.25 0 .46-.18.49-.42l.38-2.65c.61-.25 1.17-.59 1.69-.98l2.49 1c.23.09.49 0 .61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12 15.5c-1.93 0-3.5-1.57-3.5-3.5s1.57-3.5 3.5-3.5 3.5 1.57 3.5 3.5-1.57 3.5-3.5 3.5z'/%3E%3C/svg%3E");
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
    background-image: url("data:image/svg+xml,%3Csvg fill='%23444444' height='24' viewBox='0 0 24 24' width='24' xmlns='http://www.w3.org/2000/svg'%3E%3Cpath d='M0 0h24v24H0z' fill='none'/%3E%3Cpath d='M19.43 12.98c.04-.32.07-.64.07-.98s-.03-.66-.07-.98l2.11-1.65c.19-.15.24-.42.12-.64l-2-3.46c-.12-.22-.39-.3-.61-.22l-2.49 1c-.52-.4-1.08-.73-1.69-.98l-.38-2.65C14.46 2.18 14.25 2 14 2h-4c-.25 0-.46.18-.49.42l-.38 2.65c-.61.25-1.17.59-1.69.98l-2.49-1c-.23-.09-.49 0-.61.22l-2 3.46c-.13.22-.07.49.12.64l2.11 1.65c-.04.32-.07.65-.07.98s.03.66.07.98l-2.11 1.65c-.19.15-.24.42-.12.64l2 3.46c.12.22.39.3.61.22l2.49-1c.52.4 1.08.73 1.69.98l.38 2.65c.03.24.24.42.49.42h4c.25 0 .46-.18.49-.42l.38-2.65c.61-.25 1.17-.59 1.69-.98l2.49 1c.23.09.49 0 .61-.22l2-3.46c.12-.22.07-.49-.12-.64l-2.11-1.65zM12 15.5c-1.93 0-3.5-1.57-3.5-3.5s1.57-3.5 3.5-3.5 3.5 1.57 3.5 3.5-1.57 3.5-3.5 3.5z'/%3E%3C/svg%3E");
}
)stylescss";

static const char *picodom_js = R"picodomjs(
!function(e,r){"object"==typeof exports&&"undefined"!=typeof module?r(exports):"function"==typeof define&&define.amd?define(["exports"],r):r(e.picodom={})}(this,function(e){"use strict";function r(e,r){var n,t=[];for(s=arguments.length;s-- >2;)c.push(arguments[s]);for(;c.length;)if(Array.isArray(n=c.pop()))for(s=n.length;s--;)c.push(n[s]);else null!=n&&!0!==n&&!1!==n&&t.push("number"==typeof n?n+="":n);return"string"==typeof e?{type:e,props:r||{},children:t}:e(r||{},t)}function n(e,r,n,t){for(var o=l(n||(n=document.body),n.children[0],e,r);t=a.pop();)t();return o}function t(e,r){var n={};for(var t in e)n[t]=e[t];for(var t in r)n[t]=r[t];return n}function o(e,r){if("string"==typeof e)var n=document.createTextNode(e);else{var n=(r=r||"svg"===e.type)?document.createElementNS("http://www.w3.org/2000/svg",e.type):document.createElement(e.type);e.props&&e.props.oncreate&&a.push(function(){e.props.oncreate(n)});for(var t=0;t<e.children.length;t++)n.appendChild(o(e.children[t],r));for(var t in e.props)p(n,t,e.props[t])}return n}function p(e,r,n,o){if("key"===r);else if("style"===r)for(var r in t(o,n=n||{}))e.style[r]=n[r]||"";else{try{e[r]=n}catch(e){}"function"!=typeof n&&(n?e.setAttribute(r,n):e.removeAttribute(r))}}function i(e,r,n){for(var o in t(r,n)){var i=n[o],f="value"===o||"checked"===o?e[o]:r[o];i!==f&&p(e,o,i,f)}n&&n.onupdate&&a.push(function(){n.onupdate(e,r)})}function f(e,r,n){function t(){e.removeChild(r)}n&&n.onremove&&"function"==typeof(n=n.onremove(r))?n(t):t()}function u(e){if(e&&e.props)return e.props.key}function l(e,r,n,t,p,s){if(null==n)r=e.insertBefore(o(t,p),r);else if(null!=t.type&&t.type===n.type){i(r,n.props,t.props),p=p||"svg"===t.type;for(var c=t.children.length,a=n.children.length,d={},h=[],v={},y=0;y<a;y++){var g=h[y]=r.childNodes[y],m=n.children[y],b=u(m);null!=b&&(d[b]=[g,m])}for(var y=0,k=0;k<c;){var g=h[y],m=n.children[y],w=t.children[k],b=u(m);if(v[b])y++;else{var x=u(w),A=d[x]||[];null==x?(null==b&&(l(r,g,m,w,p),k++),y++):(b===x?(l(r,A[0],A[1],w,p),y++):A[0]?(r.insertBefore(A[0],g),l(r,A[0],A[1],w,p)):l(r,g,null,w,p),k++,v[x]=w)}}for(;y<a;){var m=n.children[y],b=u(m);null==b&&f(r,h[y],m.props),y++}for(var y in d){var A=d[y],B=A[1];v[B.props.key]||f(r,A[0],B.props)}}else r&&t!==r.nodeValue&&("string"==typeof t&&"string"==typeof n?r.nodeValue=t:(r=e.insertBefore(o(t,p),s=r),f(e,s,n.props)));return r}var s,c=[],a=[];e.h=r,e.patch=n});
)picodomjs";

static const char *app_js = R"appjs(

window.app = {
    cmd: function(c) { window.external.invoke_(JSON.stringify(c)); },
    setCursor: function(pos) { app.cmd({cmd: 'set_cursor', cursor: pos}); },
    setText: function(s) { app.cmd({cmd: 'set_text', text: s}); },
    setPalette: function(fg, bg) { app.cmd({cmd: 'set_palette', fg: fg, bg: bg}); },
    setPreviewSize: function(w, h) { app.cmd({cmd: 'set_preview_size', w: w, h: h}); },
    createFile: function() { app.cmd({cmd: 'create_file'}); },
    openFile: function() { app.cmd({cmd: 'open_file'}); },
    exportPDF: function() { app.cmd({cmd: 'export_pdf'}); },
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
                        title: 'Create new',
                        onclick: app.createFile,
                }),
                h('div', {
                        class: 'icon icon-open-file',
                        title: 'Open file',
                        onclick: app.openFile,
                }),
                h('div', {
                        class: 'icon icon-export-pdf',
                        title: 'Export to PDF',
                        onclick: app.exportPDF,
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
render();
)appjs";

#endif // UI_HPP