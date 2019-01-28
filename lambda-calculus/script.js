/** Print and, if possible, execute a function. */
const execute = (() => {
    let count = 0;
    const term = document.getElementById("term");

    return (func, ...arg) => {
        let html = "BAD EXEC";

        if (arg.length) {
            let res = [];
            let exec = func;

            try {
                arg.forEach(val => {
                    exec = exec(val);
                    res.push(exec);
                });
            } catch (e) {
                console.log(`Stopped executingafter "${func}" after ${res.length} arguments.`)
            }

            html = `${++count}. Executing expression:<br><code>${func}</code><br><div>for <code>${
                arg.slice(0, res.length).join(", ")
            }</code><br>Result: <code class="result">${
                res[res.length - 1]
            }${
                res.length > 1 ? `<span class="rest">[${res.slice(0, -1).join("; ")}]</span>` : ""
            }</code></div>`;
        } else {
            html = `Expression:<br><code>${func}</code>`;
        }

        dom.prepend(`<div class="expression">${html}</div>`, term);
    };
})();


// OPERATIONS

const inc = a => a + 1;
const dec = a => a - 1;

const add = a => b => a + b;
const sub = a => b => a - b;


// NAMED COMBINATORS

/** Identity function. */
const idiot = a => a;
/** Function that allies itself. */
const mockingbird = f => f(f);
/** Constant function. */
const kestrel = a => b => a;
/** Parameter flipper function. */
const cardinal = f => a => b => f(b)(a);


// BOOLEAN COMBINATORS

/** True function. */
const TRUE = kestrel;
TRUE.toString = () => "true"; // for better printing

/** False function. */
const FALSE = kestrel(idiot);
FALSE.toString = () => "false"; // for better printing

/** NOT function. */
const not = p => p(FALSE)(TRUE);
/** AND function. */
const and = p => q => p(q)(p);
/** OR function. */
const or = p => q => p(p)(q); // "or = mockingbird" works too
/** Equality function. */
const equal = p => q => p(q)(not(q));


execute(idiot, 3);
execute(mockingbird, idiot);
execute(kestrel, 1, 2);
execute(cardinal, kestrel, 1, 2);
execute(not, TRUE);
execute(not, FALSE);
execute(equal, TRUE, FALSE);
execute(equal, FALSE, TRUE);
execute(equal, FALSE, FALSE);
execute(equal, TRUE, TRUE);
