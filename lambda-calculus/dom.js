// Basic DOM manipulation.
window.dom = (function () {
    let parser = new DOMParser();

    // addNodesFn = function (parent, arrayOfNodes) {...}
    function addHtml(htmlString, parent, addNodesFn) {
        if (parent == null) {
            parent = document.body;
        }

        let doc = parser.parseFromString(htmlString, "text/html");
        let nodes = Array.prototype.slice.call(doc.firstChild.children[1].childNodes);

        addNodesFn(parent, nodes);

        return nodes;
    }

    return {
        // Returns the selected nodes in an array.
        get: function (selector) {
            if (!selector) { return []; }
            return Array.prototype.slice.call(document.querySelectorAll(selector));
        },

        /* Attach an event listener. Use in conjunction with get().
         * ex. dom.on(dom.get("p"), "click", () => {...});
        **/
        on: function (nodes, eventName, callback) {
            nodes.forEach(function (el) { el.addEventListener(eventName, callback); });
        },

        /* Create and append elements. Returns them in an array.
         * htmlString: The string that will be parsed and added.
         * (optional) parent: The new subtree will be appended here; document.body by default.
        **/
        append: function (htmlString, parent) {
            return addHtml(htmlString, parent, (parent, nodes) => {
                nodes.forEach(function (el) { parent.appendChild(el); });
            });
        },

        // Same as append() except prepends the elements.
        prepend: function (htmlString, parent) {
            return addHtml(htmlString, parent, (parent, nodes) => {
                for (let i = nodes.length - 1; i >= 0; --i) {
                    parent.insertBefore(nodes[i], parent.firstChild);
                }
            });
        },

        // Remove an array of nodes.
        remove: function (nodes) {
            nodes.forEach(function (el) {
                let par = el.parentNode;
                if (el !== par) { par.removeChild(el); }
            });
        }
    };
})();