<template lang="pug">
  .scoundCollection
    .collection(
      @drop="drop"
      :class="{ isHighlight: isHighlight }"
      @dragover="event => event.preventDefault()"
      @dragenter="highlight"
      @dragleave="unhighlight" )
      Sound(
        v-for="(sound, i) of collection"
        :draggable="false"
        :mp3File="sound.mp3File"
        class="selected"
        @destroy="destroySound(i)"
        :key="sound.key" )
    .space
      pre {{ code }}
    .space
      el-color-picker( color-format="rgb" v-model="color" size="small" )
    .space
      el-button( @click="destroySounds" ) Empty Sounds
    .space
      el-button( @click="playCollection" ) Preview
    .space
      el-button( @click="sendMessageToMQTT" ) Send to Rabbit

</template>

<script>

import mqtt from 'mqtt';
const client = mqtt.connect('ws://homebridge:8080');

client.on('connect', function () {
  // eslint-disable-next-line no-console
  console.log('connected');
});

import Sound from '@/components/Sound.vue';

const publicPath = process.env.BASE_URL;
const trimNumber = number => `000${number}`.slice(-4);

export default {
  name: 'SoundCollection',
  data: () => ({
    collection: [],
    color: null,
    isHighlight: false,
  }),
  computed: {
    code() {
      const MQTTmessage = {};
      if (this.collection.length) {
        MQTTmessage['sound'] = this.collection.map(sound => sound.mp3File);
      }
      if (this.color) {
        MQTTmessage['colors'] = this.color && this.color.match(/\d+/g);
      }
      return JSON.stringify(MQTTmessage, null, '');
    },
  },
  methods: {
    drop(event) {
      const data = event.dataTransfer.getData("fileNumber");
      this.collection.push({
        mp3File: Number(data),
        key: new Date().getTime(),
      });
      this.unhighlight(event);
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
    },
    highlight(event) {
      event.preventDefault();
      this.isHighlight = true;
    },
    unhighlight(event) {
      event.preventDefault();
      this.isHighlight = false;
    },
    destroySound(index) {
      this.collection.splice(index, 1);
    },
    destroySounds() {
      this.collection.splice(0);
    },
    sendMessageToMQTT() {
      client.publish('iot/rabbit', this.code);
    }
  },
    components: {
      Sound,
    }
}
</script>

<style lang="scss">
.collection {
  border: 2px solid black;
  border-radius: 5px;
  width: 50vw;
  min-height: 200px;
  display: flex;
  flex-wrap: wrap;
  justify-items: baseline;
  align-content: flex-start;
  &.isHighlight {
    border-style: dashed;
  }
}
.space {
  margin: 10px;
  display: flex;
  justify-content: flex-start;
}
code {
  margin: 0;
}

</style>
