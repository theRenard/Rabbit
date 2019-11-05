<template lang="pug">
  #app
    img( alt="Nabztag logo" src="./assets/logo.jpg" )
    #collectionMaker
      #sounds
        Sound( v-for="i of 63" :mp3File="i" :key="i" )
      SoundCollection

</template>

<script>

import Sound from '@/components/Sound.vue';
import SoundCollection from '@/components/SoundCollection.vue';

export default {
  name: 'app',
  components: {
    Sound,
    SoundCollection,
  }
}
</script>

<style lang="sass">
body, html
  margin: 0

#app
  font-family: 'Avenir', Helvetica, Arial, sans-serif
  -webkit-font-smoothing: antialiased
  -moz-osx-font-smoothing: grayscale
  text-align: center
  color: #2c3e50
  margin-top: 60px

#collectionMaker
  display: flex

#sounds
  width: 50vw
  display: flex
  flex-wrap: wrap
</style>
