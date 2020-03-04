// Projeto Mathematical Ramblings (mathematicalramblings.blogspot.com).

// Arquivo service worker para "https://antoniovandre.github.io/MathematicalRamblingsjs/".

// Última atualização: 04-03-2020.

var CACHE_NAME = 'Mathematical-Ramblings-cache-v1';

var urlsToCache = ['MathematicalRamblings.js'];

self.addEventListener('install', function (event) {
  event.waitUntil(
    caches.open(CACHE_NAME)
      .then(function(cache){return cache.addAll(urlsToCache);})
  );
});

self.addEventListener('activate', function (event) {
  var cacheWhitelist = ['Mathematical-Ramblings-cache-v1'];
  event.waitUntil(
    caches.keys().then(function (cacheNames) {
      return Promise.all(cacheNames.map(function (cacheName) {
        if (cacheWhitelist.indexOf(cacheName) === -1) {
          return caches.delete(cacheName);
        }
      })
      );
    })
  );
});

self.addEventListener('fetch', function (event) {
  event.respondWith(
    caches.match(event.request)
      .then(function(response) {
        if (response) {
          return response;
        }
        return fetch(event.request);
    }
    )
  );
});

self.addEventListener('message', function (event) {
  var port = event.ports[0];
  port.postMessage(eval(event.data[0]));
});

