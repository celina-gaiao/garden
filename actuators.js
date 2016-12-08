/*
 * Copyright (c) 2016 Celina Gaião
 * https://github.com/celina-gaiao/garden
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

const five = require('johnny-five');
const chipio = require('chip-io');

let colors = ['brown', 'blue', 'orange', 'green'];

exports.init = function() {

    return new Promise((resolve, reject) => {
        let solenoids = [];

        if (process.env.NODE_ENV !== 'production') {

            for (let i = 0; i < 4; i++) {
                solenoids.push({
                    id: colors[i],
                    pin: `CSID${i * 2}`,
                    on: () => {
                    },
                    off: () => {
                    },
                });
            }

            resolve(solenoids);

        } else {
            let board = new five.Board({
                io: new chipio(),
                repl: false,
            });
            board.on('ready', function () {

                for (let i = 0; i < 4; i++) {
                    solenoids[i] = new five.Relay({
                        id: colors[i],
                        pin: `CSID${i * 2}`, // use CSID0, CSID2, CSID4, CSID6
                        type: 'NO', // normally open
                    });
                    solenoids[i].off();
                }

                resolve(solenoids);
            });
        }
    });
};

