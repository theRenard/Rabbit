<template lang="pug">
  .soundCollection(
    @drop="drop"
    @click="playCollection"
    @dragover="event => event.preventDefault()" )
    Sound( :draggable="false" v-for="sound of collection" :mp3File="sound.mp3File" :key="sound.key" )

</template>

<script>

import Sound from '@/components/Sound.vue';

const publicPath = process.env.BASE_URL;
const trimNumber = number => `000${number}`.slice(-4);

export default {
  name: 'SoundCollection',
  data: () => ({
    collection: [],
  }),
  methods: {
    drop() {
      const data = event.dataTransfer.getData("fileNumber");
      this.collection.push({
        mp3File: Number(data),
        key: new Date().getTime(),
      });
    },
    playSound(sound) {
      const audioFile = `${publicPath}sounds/${trimNumber(sound.mp3File)}.mp3`;
      return new Audio(audioFile).play();
    },
    playCollection() {
      let index = 0;
      const play = () => this.playSound(this.collection[index])
        .then(setTimeout(() => {
          index += 1;
          if (index <= this.collection.length - 1) {
            play();
          }
          }, 500));
      play();
    }
  },
    components: {
      Sound,
    }
}
</script>

<style lang="sass">
.soundCollection
  background-color: black
  width: 50vw
  height: 400px
  display: flex
  flex-wrap: wrap
  justify-items: baseline
  align-content: flex-start

</style>
