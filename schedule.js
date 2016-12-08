/*
 * Copyright (c) 2016 Celina Gaião
 * https://github.com/celina-gaiao/garden
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

/**
 * @module schedule
 */

const schedule = require('node-schedule');
const debug = require('debug')('garden:schedule');

let schedules = [];

/**
 * Check solenoid values, turn the solenoid on and off.
 *
 * @param {Object} solenoid The solenoid.
 * @param {number} [solenoid.id] The unique id of the solenoid (for debug).
 * @param {number} solenoid.startTimeH The hour to call `on`.
 * @param {number} solenoid.startTimeM The minute to call `on`.
 * @param {number} solenoid.duration The interval in minutes to be on.
 * @param {Function} solenoid.on The function to call when it is time to turn
 *   the solenoid on.
 * @param {Function} solenoid.off The function to call when it is time to turn
 *   the solenoid off.
 */
function setSingleSchedule(solenoid) {

    let {startTimeH, startTimeM, duration} = solenoid;

    startTimeM = startTimeM || 0;

    if (startTimeH === undefined) {
        throw new Error('StartTimeH must be defined');
    }

    if (!duration || duration <= 0) {
        throw new RangeError('Duration must be above 0');
    }

    let s = schedule.scheduleJob(`${startTimeM} ${startTimeH} * * *`, function () {

        debug(`turning on ${solenoid.id} at ${new Date()}`);

        solenoid.on();

        setTimeout(() => {

            debug(`turning off ${solenoid.id} at ${new Date()}`);

            solenoid.off();

        }, duration * 60 * 1000);
    });

    // save schedules to the list of schedules (to allow clear)
    schedules.push(s);
}

/**
 * Sets the schedule to one or more solenoids.
 *
 * See {@link module:schedule~setSingleSchedule} for more information on
 * solenoid params.
 *
 * @param {Object[]|Object} solenoids One or an Array of solenoid objects.
 */
exports.setSchedule = function (solenoids) {

    solenoids = Array.isArray(solenoids) ? solenoids : [solenoids];

    for (let solenoid of solenoids) {
        setSingleSchedule(solenoid);
    }
}

/**
 * Convenience method for testing (clears all schedules).
 */
exports.cancelAll = function () {
    schedules.map((s) => {
        s.cancel();
    });
};
