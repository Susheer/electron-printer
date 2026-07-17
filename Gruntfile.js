module.exports = function (grunt) {

    const pkg = require("./package.json");

    const buildMatrix = pkg.buildMatrix.platforms;

    const shell = {};
    const copy = {};

    const buildTasks = [];
    const buildPreTasks = [];

    buildMatrix.forEach(platform => {
        if (platform.os !== process.platform) {
            return;
        }

        platform.architectures.forEach(arch => {
            shell[`node-gyp-${arch}`] = {
                command: `node-gyp rebuild --arch=${arch}`
            };
            shell[`node-pre-gyp-${arch}`] = {
                command: `node-pre-gyp configure build package --target_arch=${arch}`
            };
            copy[arch] = {
                files: [
                    {
                        src: "build/Release/electron-printer.node",
                        dest: `lib/electron-printer-${process.platform}-${arch}.node`
                    },
                    {
                        src: "binding.js",
                        dest: "lib/binding.js"
                    },
                    {
                        src: "index.js",
                        dest: "lib/index.js"
                    }
                ]
            };
            grunt.registerTask(`build-${arch}`, [
                `shell:node-gyp-${arch}`,
                `copy:${arch}`
            ]);
            grunt.registerTask(`build-pre-${arch}`, [
                `shell:node-pre-gyp-${arch}`,
                `copy:${arch}`
            ]);
            buildTasks.push(`build-${arch}`);
            buildPreTasks.push(`build-pre-${arch}`);
        });
    });

    shell["upload-binaries"] = {
        command: "node-pre-gyp-github publish"
    };
    grunt.initConfig({
        shell,
        copy
    });
    grunt.loadNpmTasks("grunt-contrib-jshint");
    grunt.loadNpmTasks("grunt-shell");
    grunt.loadNpmTasks("grunt-contrib-copy");
    grunt.registerTask("build", buildTasks);
    grunt.registerTask("build-pre", buildPreTasks);
    grunt.registerTask("upload", [
        "shell:upload-binaries"
    ]);
    grunt.registerTask("release", [
        "build-pre"
        // "upload"
    ]);
};