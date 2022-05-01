<template>
  <div>
    <input type="file" v-on:change="handleFile" />
    <p>帧率 {{ fps }}</p>
    <canvas id="ccc" width="800" height="800" />
  </div>
</template>

<script>
const ipcRenderer = window.ipcRenderer;

export default {
  name: "BitMap",
  props: {},
  data() {
    return {
      ctx: null,
      avgRenderMs: 0,
      alpha: 1 / 100,
      minRenderTime: 1000 / 60,
      loaded: false,
    };
  },

  methods: {
    sleep(ms) {
      return new Promise((resolve) => setTimeout(resolve, ms));
    },

    async drawImage() {
      const start = performance.now();
      if (this.loaded) {
        const buf = ipcRenderer.render();
        let img = this.ctx.createImageData(800, 800);
        img.data.set(buf);
        this.ctx.putImageData(img, 0, 0);
      } else {
        await this.sleep(this.minRenderTime);
      }

      const end = performance.now();
      if (this.avgRenderMs === 0) {
        this.avgRenderMs = end - start;
      } else {
        this.avgRenderMs =
          this.avgRenderMs * (1 - this.alpha) + (end - start) * this.alpha;
      }
    },

    handleFile(e) {
      this.loaded = ipcRenderer.loadFile(e.target.files[0].path);
      ipcRenderer.send("FileUrl", e.target.files[0].path);
    },
  },

  mounted() {
    const c = document.getElementById("ccc");
    this.ctx = c.getContext("2d");

    setInterval(this.drawImage, 1);

    ipcRenderer.receive("Draw", (_, buf) => {
      this.drawImage(buf);
    });
  },

  computed: {
    fps() {
      if (this.avgRenderMs === 0) {
        return `  0`;
      }
      return `${Math.round(1000 / this.avgRenderMs)}`;
    },
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
</style>
