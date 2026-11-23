document.documentElement.classList.add('js');
const $ = s=>document.querySelector(s);
const chapters=[...document.querySelectorAll('.chapter')];
const dialog=$('#search-dialog');
let activeChapter=null, observer=null, searchIndex=null;
const escapeHTML=s=>s.replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const remember=(key,value)=>{try{localStorage.setItem(key,value)}catch{}};
const mobile=matchMedia('(max-width:780px)');
function syncDrawer(){ $('#sidebar').inert=mobile.matches&&!document.body.classList.contains('menu-open'); }
mobile.addEventListener('change',()=>{if(!mobile.matches)closeMenu();syncDrawer()});
function closeMenu(){const wasOpen=document.body.classList.contains('menu-open');document.body.classList.remove('menu-open');$('#drawer-close').hidden=true;$('#menu').setAttribute('aria-expanded','false');syncDrawer();if(wasOpen&&mobile.matches)$('#menu').focus();}
function currentHeading(id){document.querySelectorAll('.nav-sub,.page-toc a').forEach(a=>{const current=a.hash==='#'+id;a.classList.toggle('current',current);if(current)a.setAttribute('aria-current','location');else a.removeAttribute('aria-current')});}
function progress(){if(!activeChapter)return;const rect=activeChapter.getBoundingClientRect();const height=activeChapter.scrollHeight-innerHeight+100;const percent=height<=0?100:Math.max(0,Math.min(100,Math.round((100-rect.top)/height*100)));$('#progress-label').textContent=`已读 ${percent}%`;$('#progress-fill').style.width=percent+'%';}
function navigate(){
 let id;try{id=decodeURIComponent(location.hash.slice(1))}catch{id='intro'}
 if(id==='reading'&&activeChapter){$('#reading').focus();scrollTo({top:0,behavior:'instant'});return}
 if(!id||id==='reading') id='intro';
 const target=document.getElementById(id);
 const chapter=target?.closest('.chapter')||chapters[0];
 if(chapter!==activeChapter){
  activeChapter=chapter;
  chapters.forEach(c=>c.hidden=c!==chapter);
  document.querySelectorAll('[data-chapter]').forEach(a=>{const selected=a.dataset.chapter===chapter.id;a.classList.toggle('active',selected);if(selected){a.setAttribute('aria-current','page');a.closest('details').open=true}else a.removeAttribute('aria-current')});
  $('#breadcrumb').textContent=chapter.dataset.title;
  $('#read-time').textContent=`约 ${Math.max(1,Math.ceil(chapter.textContent.length/450))} 分钟`;
  document.title=`${chapter.dataset.title} · JLUCS Speedrun`;
  const headings=[...chapter.querySelectorAll('h2,h3')];
  $('#toc-links').replaceChildren(...headings.map(h=>{const a=document.createElement('a');a.href='#'+h.id;a.textContent=h.textContent.replace(/#$/,'');return a}));
  const n=chapters.indexOf(chapter);
  $('#page-nav').innerHTML=(n>0?`<a href="#${chapters[n-1].id}"><small>上一章</small>${escapeHTML(chapters[n-1].dataset.title)}</a>`:'<span></span>')+(n<chapters.length-1?`<a href="#${chapters[n+1].id}"><small>下一章</small>${escapeHTML(chapters[n+1].dataset.title)}</a>`:'');
  observer?.disconnect();
  observer=new IntersectionObserver(entries=>{for(const entry of entries)if(entry.isIntersecting)currentHeading(entry.target.id)},{rootMargin:'-90px 0px -65% 0px',threshold:0});
  chapter.querySelectorAll('h2,h3').forEach(h=>observer.observe(h));
 }
 closeMenu();
 requestAnimationFrame(()=>{if(target&&target!==chapter)target.scrollIntoView({behavior:'instant',block:'start'});else scrollTo({top:0,behavior:'instant'});currentHeading(target?.id||'');progress()});
}
$('#menu').addEventListener('click',()=>{const open=!document.body.classList.contains('menu-open');document.body.classList.toggle('menu-open',open);$('#drawer-close').hidden=!open;$('#menu').setAttribute('aria-expanded',String(open));syncDrawer();if(open)$('#sidebar a').focus()});
$('.skip').addEventListener('click',e=>{e.preventDefault();$('#reading').focus();scrollTo({top:0,behavior:'instant'})});
$('#drawer-close').addEventListener('click',closeMenu);
document.addEventListener('keydown',e=>{if(e.key==='Escape'){closeMenu();if(dialog.open)dialog.close()}});
$('#theme').addEventListener('click',()=>{const theme=document.documentElement.dataset.theme==='dark'?'light':'dark';document.documentElement.dataset.theme=theme;remember('jlu-theme',theme);$('#theme').setAttribute('aria-label',theme==='dark'?'切换浅色模式':'切换深色模式')});
$('#back-top').addEventListener('click',e=>{e.preventDefault();scrollTo({top:0,behavior:'smooth'})});
window.addEventListener('hashchange',navigate);window.addEventListener('scroll',progress,{passive:true});
document.querySelectorAll('.chapter a[href^="http"]').forEach(a=>{a.target='_blank';a.rel='noopener noreferrer'});
async function openSearch(){dialog.showModal();$('#search-input').focus();if(!searchIndex){try{const res=await fetch('./search.json');if(!res.ok)throw new Error('fetch');searchIndex=await res.json();renderSearch()}catch{$('#search-status').textContent='搜索内容加载失败，请关闭后重试。'}}}
function renderSearch(){
 const q=$('#search-input').value.trim();const results=$('#search-results');results.replaceChildren();
 if(!q){$('#search-status').textContent='输入关键词，搜索全部章节。';return}
 if(!searchIndex){$('#search-status').textContent='正在加载搜索内容…';return}
 const terms=q.toLocaleLowerCase().split(/\s+/);
 const hits=searchIndex.filter(s=>terms.every(t=>(s.title+' '+s.text).toLocaleLowerCase().includes(t))).sort((a,b)=>Number(terms.some(t=>b.title.toLocaleLowerCase().includes(t)))-Number(terms.some(t=>a.title.toLocaleLowerCase().includes(t)))).slice(0,30);
 $('#search-status').textContent=hits.length?`找到 ${hits.length}${hits.length===30?' 条以内的':''} 个相关小节`:'没有找到相关内容，试试更短的关键词。';
 for(const hit of hits){const a=document.createElement('a');a.className='search-result';a.href='#'+hit.id;const strong=document.createElement('strong');strong.textContent=hit.title;const p=document.createElement('p');const pos=Math.max(0,hit.text.toLocaleLowerCase().indexOf(terms[0])-32);p.textContent=(pos?'…':'')+hit.text.slice(pos,pos+130)+'…';a.append(strong,p);a.addEventListener('click',()=>{dialog.close();if(location.hash===a.hash)navigate()});results.append(a)}
}
$('#search-open').addEventListener('click',openSearch);$('#search-close').addEventListener('click',()=>dialog.close());
dialog.addEventListener('click',e=>{if(e.target===dialog){const r=dialog.getBoundingClientRect();if(e.clientX<r.left||e.clientX>r.right||e.clientY<r.top||e.clientY>r.bottom)dialog.close()}});
$('#search-input').addEventListener('input',renderSearch);$('#search-input').addEventListener('keydown',e=>{if(e.key==='Enter'){const first=$('.search-result');if(first){e.preventDefault();first.click()}}});
document.addEventListener('keydown',e=>{if((e.ctrlKey||e.metaKey)&&e.key.toLowerCase()==='k'){e.preventDefault();if(!dialog.open)openSearch()}});
navigate();
