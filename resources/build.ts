import fs from "fs";
import path from "path";
import Markdown from "markdown-it";
import Mustache from "mustache";
import { globSync } from "glob";

// args
const args = process.argv.slice(2);

// initialize markdown
const md = Markdown({ html: true })

// string constants
const build_path = args.includes("debug") ? "build/Debug" : "build/Release";
const content_path = "resources/content";
const res_mustache_path = "resources/mustache/";
const js_output_path = "website/assets/js";

// mustache partials
const page = fs.readFileSync(`${res_mustache_path}/page.mustache`, "utf8");
const meta = fs.readFileSync(`${res_mustache_path}/meta.mustache`, "utf8");
const header = fs.readFileSync(`${res_mustache_path}/header.mustache`, "utf8");
const footer = fs.readFileSync(`${res_mustache_path}/footer.mustache`, "utf8");

// helper functions
function _copyDirectory(source: string, destination: string) {
  fs.cp(source, destination, { recursive: true, force: true }, (err) => { if (err) { console.error(`Error copying directory from ${source} to ${destination}:`, err); } });
}
function _copyFile(source: string, destination: string) {
  fs.copyFile(source, destination, (err) => { if (err) { console.error(`Error copying file from ${source} to ${destination}:`, err); } });
}
function _writeFile(destination: string, content: string) {
  fs.writeFile(destination, content, (err) => { if (err) { console.error(`Error writting file to ${destination}:`, err); } });
}

// builds a single page
function _buildPage(body: string) {
  // render markdown to html
  const view = { body: md.render(body) };
  const partials = { meta: meta, header: header, footer: footer };

  // template using mustache
  return Mustache.render(page, view, partials);
}

function _buildWebsite() {
  console.log("Building website...");

  // create website directory for output
  if (!fs.existsSync("website")) {
    fs.mkdirSync("website");
  }
  if (!fs.existsSync("website/assets")) {
    fs.mkdirSync("website/assets", { recursive: true });
  }

  // glob all `.md` files
  const md_glob = globSync(`${content_path}/*.md`);
  md_glob.forEach((file: string) => {
    const name = path.parse(file).name;
    const body = fs.readFileSync(file, "utf8");
    const content = md.render(_buildPage(body));

    // copy to output
    _writeFile(`website/${name}.html`, content);
  });

  // glob all `.html` files
  const html_glob = globSync(`${content_path}/*.html`);
  html_glob.forEach((file: string) => {
    const name = path.parse(file).name;

    // copy to output
    _copyFile(file, `website/${name}.html`);
  });

  // glob all `.js` files
  const wasm_glob = globSync(`${build_path}/*.js`);
  wasm_glob.forEach((file: string) => {
    const name = path.parse(file).name;

    // copy to output
    _copyFile(`${build_path}/${name}.js`, `${js_output_path}/${name}.js`);
  });

  // copy to output
  _copyDirectory("resources/assets", "website/assets");
}

// main.js
_buildWebsite();