const files = [
    'MainGui',
    'OSC',
    'WaveEditor'
];
const fs = require('fs'),
    crypto = require('crypto'),
    tpl = fs.readFileSync(__dirname + "/template.tpl", "utf8"),
    lastVersion = fs.readFileSync(__dirname + "/lastVersions.json", "utf8"),
    outDir = "./../../Plugin/Source/VeNo/Generated/GUIFiles.cpp";


let mapping = {};
try {
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
    const content = fs.readFileSync(`${__dirname}/files/${file}.gui`, "utf8");
    const hash = getHash(content);
    if (!mapping[file] || mapping[file] !== hash) {
        hashChanged = true;
        mapping[file] = hash;
    }
    out += getContent(file, content) + "\n";
}

if (hashChanged) {
    fs.writeFileSync(outDir, tpl.replace("$s$", out));
    fs.writeFileSync(__dirname + "/lastVersions.json", JSON.stringify(mapping, null, 4));
}
