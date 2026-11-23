# 网页版维护

网站正文自动读取仓库根目录 `README.md`，不维护第二份文案。修改原 README 后，GitHub Actions 自动构建并部署 GitHub Pages。

## 本地构建

需要 Node.js 22，无需安装依赖：

```sh
node website/build.mjs
```

生成目录为 `website/dist/`。用任意静态 HTTP 服务预览此目录。所有资源使用相对路径，兼容 GitHub Pages 的 `/JLUCS_Speedrun/` 子路径。

页面按原文 `<details>` 拆为前言、四章与后记。小节目录、搜索索引与阅读时间自动生成；图片以原始字节随站点发布，来源记录在 `assets/SOURCES.md`。

`vendor/marked.esm.js` 为 Marked 的本地构建文件，避免读者访问网站时依赖外部脚本 CDN。其许可证保存在同目录。

## 部署

仓库 Settings → Pages → Build and deployment 选择 GitHub Actions；工作流为 `.github/workflows/pages.yml`。预期网址为 https://xiaoc7r.github.io/JLUCS_Speedrun/ 。
