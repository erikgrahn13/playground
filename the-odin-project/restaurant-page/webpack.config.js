const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");
const { type } = require("os");

module.exports = {
    mode: "production",
    entry: "./src/index.js",
    devtool: "eval-source-map",
    devServer: {
        watchFiles: ["./src/template.html"]
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: "./src/template.html"
        })
    ],
    module: {
        rules: [
            {
                test: /\.(jpg)$/i,
                type: "asset/resource",
            },
            {
                test: /\.css$/i,
                use: ["style-loader", "css-loader"]
            },
            {
                test: /\.ttf$/i,
                type: "asset/resource",
            }
        ]
    }
}