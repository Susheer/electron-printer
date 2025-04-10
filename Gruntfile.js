module.exports = function(grunt) {
    const versions = {
        ia32: [
            "23.11.0",
            "22.14.0",
            "21.7.3",
            "20.19.0",
            "19.9.0",
            "18.20.8"
        ],
        x64: [
            "23.11.0",
            "22.14.0",
            "21.7.3",
            "20.19.0",
            "19.9.0",
            "18.20.8"
        ],
    }
    const target_arch = ["ia32","x64"];
    const shell = {};
    
    target_arch.forEach((arch) => {
        versions[arch].forEach(version => {
            shell[`node-pre-gyp-${arch}-v${version+""}`] = {
                command: `node-pre-gyp configure build package --target_arch=${arch} --target=${version}`
            };
        });
    });

    grunt.initConfig({
        shell: {
            ...shell,
            'upload-binaries': {
                command: 'node-pre-gyp-github publish'
            }
        },
        copy: {
            ia32: {
                files: [
                    {src: 'build/Release/electron-printer.node', dest: 'lib/electron-printer-' + process.platform + '-ia32.node'},
                    {src: 'binding.js', dest: 'lib/binding.js'},
                    {src: 'index.js', dest: 'lib/index.js'}
                ]
            },
            x64: {
                files: [
                    {src: 'build/Release/electron-printer.node', dest: 'lib/electron-printer-' + process.platform + '-x64.node'},
                    {src: 'binding.js', dest: 'lib/binding.js'},
                    {src: 'index.js', dest: 'lib/index.js'}
                ]
            }
        }
    });

    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.loadNpmTasks('grunt-shell');
    grunt.loadNpmTasks('grunt-contrib-copy');

    grunt.registerTask('build-pre-ia32', [
        versions['ia32'].map((ver)=>`shell:node-pre-gyp-ia32-v${ver}`),
        'copy:ia32'
    ]);

    grunt.registerTask('build-pre-x64', [
        'shell:node-pre-gyp-x64',
        'copy:x64'
    ]);

    grunt.registerTask('build-pre', [
        ...versions['x64'].map((ver)=>`shell:node-pre-gyp-x64-v${ver}`),
        'build-pre-x64'
    ]);

    grunt.registerTask('upload', [
        'shell:upload-binaries'
    ]);

    grunt.registerTask('release', [
        'build-pre',
        'upload'
    ]);
};
