const files = [
    'MainGui',
    'OSC'
];
const fs = require('fs'),
    tpl = fs.readFileSync(__dirname + "/template.tpl", "utf8"),
    outDir = "./../../Plugin/Source/VeNo/Generated/GUIFiles.cpp";


function getContent(name, cont) {
    return `std::string Files::${name} = R""(${cont})"";`;
}

let out = "";
for (const file of files) {
    const content = fs.readFileSync(`${__dirname}/files/${file}.gui`, "utf8");
    out += getContent(file, content) + "\n";
}

fs.writeFileSync(outDir, tpl.replace("$s$", out));