const fs = require('fs'),
    crypto = require('crypto'),
    tplHeader = fs.readFileSync(__dirname + "/header.tpl", "utf8"),
    tplContent = fs.readFileSync(__dirname + "/content.tpl", "utf8"),
    outFileTpl = "./../../Plugin/Source/VeNo/GUI/GUIFiles.",
    assetDir = `${__dirname}/../../Plugin/Assets/UIFiles`;

const files = fs.readdirSync(assetDir);
let mapping = {};
try {
    const lastVersion = fs.readFileSync(__dirname + "/lastVersions.json", "utf8")
    mapping = JSON.parse(lastVersion);
} catch (e) {

}

function getContent(name, cont) {
    return `std::string Files::${name} = R""(${cont})"";`;
}

function getHash(content) {
    const hash = crypto.createHash('SHA256');
    hash.setEncoding('hex');
    hash.write(content);
    hash.end();
    return hash.read();
}

let hashChanged = false;

let out = "";
for (const file of files) {
    const name = file.replace(".vui", "");
    const content = fs.readFileSync(`${assetDir}/${file}`, "utf8");
    const hash = getHash(content);
    if (!mapping[name] || mapping[name] !== hash) {
        hashChanged = true;
        mapping[name] = hash;
    }
    out += `      {"${name}", VeNo::UI::${name}_vui},\n`;
}

if (hashChanged) {
    out.length -= 3;
    fs.writeFileSync(outFileTpl + "h", tplHeader.replace("$d$", files.length.toString(10)));
    fs.writeFileSync(outFileTpl + "cpp", tplContent.replace("$s$", out));
}
fs.writeFileSync(__dirname + "/lastVersions.json", JSON.stringify(mapping, null, 4));
