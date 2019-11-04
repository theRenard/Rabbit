<template lang="pug">
  #app
    img( alt="Nabztag logo" src="./assets/logo.jpg" )
    Sound( v-for="i of 63" :mp3File="i" :key="i" ) {{ i }}
</template>

<script>

import Sound from '@/components/Sound.vue';

export default {
  name: 'app',
  components: {
    Sound,
  }
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
