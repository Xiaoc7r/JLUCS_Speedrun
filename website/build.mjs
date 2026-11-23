import fs from 'node:fs/promises';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { createHash } from 'node:crypto';
import { marked } from './vendor/marked.esm.js';

const dir = path.dirname(fileURLToPath(import.meta.url));
const root = path.dirname(dir);
const dist = path.join(dir, 'dist');
await fs.mkdir(path.join(dist, 'assets'), {recursive:true});
const source = await fs.readFile(path.join(root, 'README.md'), 'utf8');
const sourceHash = createHash('sha256').update(source).digest('hex');
let markdown = source.replace(/^\uFEFF/, '');
const imageSources = [...new Set([...markdown.matchAll(/https:\/\/github\.com\/user-attachments\/assets\/[\w-]+/g)].map(m=>m[0]))];
for (const [i,url] of imageSources.entries()) {
  const filename = `figure-${i+1}.png`;
  await fs.copyFile(path.join(dir, 'assets', filename), path.join(dist, 'assets', filename));
  markdown = markdown.replaceAll(url, `./assets/${filename}`);
}
const escape = s => s.replace(/[&<>"']/g, ch=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
const clean = s => s.replace(/<[^>]+>/g,'').replace(/\*\*/g,'').replace(/&amp;/g,'&').replace(/&lt;/g,'<').replace(/&gt;/g,'>').replace(/&quot;/g,'"').replace(/&#39;/g,"'").trim();
const matches = [...markdown.matchAll(/<details>\s*<summary><h2>(.*?)<\/h2><\/summary>([\s\S]*?)<\/details>/g)];
if (matches.length !== 5) throw new Error(`Expected 5 README chapters; received ${matches.length}. Update parser before publishing.`);
const intro = markdown.slice(0,matches[0].index).replace(/<a id="part-1"><\/a>/g,'').replace(/\n---\s*$/,'');
const chapters = [{id:'intro',title:'前言',markdown:intro}, ...matches.map((m,i)=>({id:i<4?`part-${i+1}`:'afterword',title:clean(m[1]),markdown:`## ${m[1]}\n\n${m[2]}`}))];
const index = [];
for (const chapter of chapters) {
  let headingIndex=0;
  chapter.headings=[];
  const renderer = new marked.Renderer();
  renderer.heading = ({tokens,depth}) => {
    const text = marked.Parser.parseInline(tokens);
    const label=clean(text);
    const number = label.match(/^(\d+\.\d+)\b/);
    const id = number ? `s-${number[1].replace('.','-')}` : `${chapter.id}-h-${++headingIndex}`;
    chapter.headings.push({id,title:label,depth});
    return `<h${depth} id="${id}">${text}<a class="heading-anchor" href="#${id}" aria-label="链接到此节">#</a></h${depth}>\n`;
  };
  chapter.html = marked.parse(chapter.markdown,{gfm:true,renderer});
  let current = {chapter:chapter.id,id:chapter.id,title:chapter.title,text:''};
  for(const line of chapter.markdown.split('\n')) {
    const h=line.match(/^### (.*)/);
    if(h) {
      index.push(current);
      const label=clean(h[1]);
      current={chapter:chapter.id,id:chapter.headings.find(x=>x.title===label)?.id||chapter.id,title:label,text:''};
    } else current.text += ' '+clean(line.replace(/!?\[([^\]]*)\]\([^)]*\)/g,'$1'));
  }
  index.push(current);
}
const icon = (name) => {
 const paths={book:'<path d="M4 4h6a3 3 0 0 1 3 3v14a4 4 0 0 0-4-3H4z"/><path d="M20 4h-4a3 3 0 0 0-3 3v14a4 4 0 0 1 4-3h3z"/>',search:'<circle cx="10.5" cy="10.5" r="6.5"/><path d="m16 16 4 4"/>',menu:'<path d="M4 6h16M4 12h16M4 18h16"/>',sun:'<circle cx="12" cy="12" r="4"/><path d="M12 2v2m0 16v2M2 12h2m16 0h2M5 5l1 1m12 12 1 1M5 19l1-1M18 6l1-1"/>',github:'<path d="M9 19c-4 1-4-2-6-2m12 5v-4a3 3 0 0 0-1-2c3 0 6-1 6-5a4 4 0 0 0-1-3 4 4 0 0 0 0-4s-1 0-4 2a13 13 0 0 0-6 0C6 4 5 4 5 4a4 4 0 0 0 0 4 4 4 0 0 0-1 3c0 4 3 5 6 5a3 3 0 0 0-1 2v4"/>',close:'<path d="m6 6 12 12M6 18 18 6"/>'};
 return `<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">${paths[name]}</svg>`;
};
const nav=chapters.map(c=>`<details class="nav-group" open><summary><a href="#${c.id}" data-chapter="${c.id}">${escape(c.title)}</a>${c.headings.some(h=>h.depth===3)?'<span class="chevron"></span>':''}</summary>${c.headings.filter(h=>h.depth===3).map(h=>`<a class="nav-sub" href="#${h.id}">${escape(h.title)}</a>`).join('')}</details>`).join('');
const html=`<!doctype html><html lang="zh-CN"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><meta name="theme-color" content="#faf9f5"><meta name="description" content="吉大计算机就业导向攻略。方向选择、能力准备、实习秋招路线，以及求职中的常见误区。"><title>JLUCS Speedrun · 吉大就业指南</title><link rel="icon" href="./favicon.svg" type="image/svg+xml"><link rel="stylesheet" href="./style.css"><script>try{document.documentElement.dataset.theme=localStorage.getItem('jlu-theme')||'light'}catch{}</script><script defer src="./app.js"></script></head><body>
<a class="skip" href="#reading">跳到正文</a><header class="topbar"><button id="menu" class="icon-button mobile-menu" aria-label="打开章节目录" aria-controls="sidebar" aria-expanded="false">${icon('menu')}</button><a class="brand" href="#intro"><span class="brand-mark">${icon('book')}</span><span>JLUCS <b>Speedrun</b><small>吉大就业指南</small></span></a><div class="header-actions"><a class="resume-link" href="https://xiaoc7r.github.io/DiyJobResume/" target="_blank" rel="noopener">简历工坊</a><button id="search-open" class="search-trigger">${icon('search')}<span>搜索攻略</span><kbd>Ctrl K</kbd></button><button id="theme" class="icon-button" aria-label="切换深色模式">${icon('sun')}</button><a class="icon-button" href="https://github.com/Xiaoc7r/JLUCS_Speedrun" aria-label="GitHub 仓库" target="_blank" rel="noopener">${icon('github')}</a></div></header>
<div class="layout"><aside id="sidebar" class="sidebar" aria-label="章节目录"><div class="sidebar-title">阅读目录</div><nav>${nav}</nav><div class="sidebar-bottom"><a href="https://github.com/Xiaoc7r/JLUCS_Speedrun/issues" target="_blank" rel="noopener">提问与反馈</a><span>写给后来者的一份攻略</span></div></aside><button class="drawer-shade" id="drawer-close" aria-label="关闭目录" hidden></button><main id="reading" tabindex="-1"><div class="reading-bar"><span id="breadcrumb">前言</span><span id="read-time"></span></div>${chapters.map(c=>`<article class="chapter" id="${c.id}" data-title="${escape(c.title)}">${c.html}</article>`).join('')}<nav id="page-nav" class="page-nav" aria-label="章节翻页"></nav><footer class="article-footer"><a href="https://github.com/Xiaoc7r/JLUCS_Speedrun/edit/main/README.md" target="_blank" rel="noopener">在 GitHub 上编辑原文</a><a href="#reading" id="back-top">回到顶部</a></footer></main><aside class="page-toc" aria-label="本章目录"><div class="toc-title">本章目录</div><nav id="toc-links"></nav><div class="reading-progress"><span id="progress-label">已读 0%</span><div><i id="progress-fill"></i></div></div></aside></div>
<dialog id="search-dialog" aria-label="搜索全文"><div class="search-head">${icon('search')}<input id="search-input" type="search" placeholder="搜索标题、实习、考研、Offer…" aria-label="搜索关键词" autocomplete="off"><button id="search-close" class="icon-button" aria-label="关闭搜索">${icon('close')}</button></div><div id="search-status" role="status">输入关键词，搜索全部章节。</div><div id="search-results"></div><div class="search-foot"><span>Enter 打开首条结果</span><span>Esc 关闭</span></div></dialog><noscript><style>.sidebar,.page-toc,.header-actions{display:none}.layout{display:block}main{max-width:800px;margin:auto}.chapter{display:block!important}</style></noscript></body></html>`;
await fs.writeFile(path.join(dist,'index.html'),html.replace('id="search-open"','id="search-open" aria-label="搜索攻略"'));
await fs.writeFile(path.join(dist,'search.json'),JSON.stringify(index));
await fs.writeFile(path.join(dist,'build-info.json'),JSON.stringify({source:'README.md',sha256:sourceHash,chapters:chapters.length,headings:chapters.reduce((n,c)=>n+c.headings.length,0),images:imageSources.length}));
await fs.writeFile(path.join(dist,'.nojekyll'),'');
for(const file of ['style.css','app.js','favicon.svg']) await fs.copyFile(path.join(dir,file),path.join(dist,file));
console.log(JSON.stringify({dist,chapters:chapters.length,searchSections:index.length,images:imageSources.length,sourceHash},null,2));
