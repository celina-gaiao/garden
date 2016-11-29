/*
 * Copyright (c) 2016 Celina Gaião
 * https://github.com/celina-gaiao/garden
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

const sinon = require('sinon');
const expect = require('chai').expect;
const Schedule = require('../schedule');

describe.only('Schedule', function () {

    beforeEach(function () {
        let now = new Date();
        now.setHours(7);
        now.setMinutes(0);
        now.setSeconds(0);
        now.setMilliseconds(0);

        this.clock = sinon.useFakeTimers(now.getTime());
    });

    afterEach(function () {
        this.clock.restore();
        Schedule.cancelAll();
    });

    describe('single schedule', function () {

        it('should work with single schedule', function () {
            let solenoid = {
                startTimeH: 7,
                startTimeM: 2,
                duration: 5,
                on: sinon.stub(),
                off: sinon.stub(),
            };

            Schedule.setSchedule(solenoid);
            expect(solenoid.on.notCalled).to.be.true;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 2); // +2 minutes
            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 4); // +4 minutes
            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 1); // +1 minutes
            expect(solenoid.off.calledOnce).to.be.true;
            this.clock.tick(1000 * 60 * 3); // +3 minutes
            expect(solenoid.off.calledOnce).to.be.true;
        });

        it('should be all off if it is not time to work yeat', function () {
            let solenoid = {
                startTimeH: 10,
                startTimeM: 2,
                duration: 5,
                on: sinon.stub(),
                off: sinon.stub(),
            };

            Schedule.setSchedule(solenoid);
            expect(solenoid.on.calledOnce).to.be.false;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 2); // +2 minutes
            expect(solenoid.on.calledOnce).to.be.false;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 4); // +4 minutes
            expect(solenoid.on.calledOnce).to.be.false;
            expect(solenoid.off.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 1); // +1 minutes
            expect(solenoid.on.calledOnce).to.be.false;
            this.clock.tick(1000 * 60 * 3); // +3 minutes
            expect(solenoid.off.calledOnce).to.be.false;
        });

        it('should work twice in two days', function () {
            let solenoid = {
                startTimeH: 7,
                startTimeM: 2,
                duration: 5,
                on: sinon.stub(),
                off: sinon.stub(),
            };

            Schedule.setSchedule(solenoid);

            this.clock.tick(1000 * 30); // +30 secs
            expect(solenoid.on.calledOnce).to.be.false;
            expect(solenoid.off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60 * 2); // +2 minutes, 2:30s
            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60 * 4); // +4 minutes, 6:30s
            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60 * 3); // +3 minutes, 0:9:30s
            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.true;

            // second day
            this.clock.tick(1000 * 60 * 60 * 23); // 23:09:30
            this.clock.tick(1000 * 60 * 50); // 23:59:30
            this.clock.tick(1000 * 60 * 2); // 00:1:30

            expect(solenoid.on.calledOnce).to.be.true;
            expect(solenoid.off.calledOnce).to.be.true;

            this.clock.tick(1000 * 60 * 2); // +2 minutes, 3:30s
            expect(solenoid.on.calledTwice).to.be.true;
            expect(solenoid.off.calledOnce).to.be.true;

            this.clock.tick(1000 * 60 * 4); // +4 minutes, 7:30s
            expect(solenoid.on.calledTwice).to.be.true;
            expect(solenoid.off.calledTwice).to.be.true;

        });

        it('should throw Error when duration is 0', function () {

            let solenoid = {
                startTimeH: 7,
                startTimeM: 2,
                duration: 0,
            };

            expect(Schedule.setSchedule.bind(null, solenoid)).to.throw(RangeError, /Duration must be above 0/);

            solenoid = {
                startTimeH: 7,
                startTimeM: 2,
            };

            expect(Schedule.setSchedule.bind(null, solenoid)).to.throw(RangeError, /Duration must be above 0/);

        });
    });

    describe('multiple schedules', function () {

        it('should work with all at once', function () {

            let solenoids = [
                {
                    startTimeH: 7,
                    startTimeM: 2,
                    duration: 5,
                    on: sinon.stub(),
                    off: sinon.stub(),
                },
                {
                    startTimeH: 7,
                    startTimeM: 2,
                    duration: 5,
                    on: sinon.stub(),
                    off: sinon.stub(),
                },
                {
                    startTimeH: 7,
                    startTimeM: 2,
                    duration: 5,
                    on: sinon.stub(),
                    off: sinon.stub(),
                },
            ];

            Schedule.setSchedule(solenoids);

            this.clock.tick(1000 * 60 * 3); // +3 minutes
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.false;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60 * 5); // +5 minutes
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.true;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.true;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.true;

        });

        it('should work when overlaped', function () {

            let on = [];
            let off = [];

            let solenoids = [
                    {
                        startTimeH: 7,
                        startTimeM: 1,
                        duration: 5,
                        on: sinon.stub(),
                        off: sinon.stub(),
                    },
                    {
                        startTimeH: 7,
                        startTimeM: 2,
                        duration: 2,
                        on: sinon.stub(),
                        off: sinon.stub(),
                    },
                    {
                        startTimeH: 7,
                        startTimeM: 3,
                        duration: 2,
                        on: sinon.stub(),
                        off: sinon.stub(),
                    },
                ]
                ;

            Schedule.setSchedule(solenoids);

            this.clock.tick(1000 * 30); // +30 secs
            expect(solenoids[0].on.calledOnce).to.be.false;
            expect(solenoids[0].off.calledOnce).to.be.false;
            //  expect(solenoids[1].on.calledOnce).to.be.false;
            expect(solenoids[1].off.calledOnce).to.be.false;
            expect(solenoids[2].on.calledOnce).to.be.false;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60); // +1 min, 1:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.false;
            expect(solenoids[1].off.calledOnce).to.be.false;
            expect(solenoids[2].on.calledOnce).to.be.false;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60); // +1 min, 2:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.false;
            expect(solenoids[2].on.calledOnce).to.be.false;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60); // +1 min, 3:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.false;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60); // +1 min, 4:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.true;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.false;

            this.clock.tick(1000 * 60); // +1 min, 5:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.false;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.true;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.true;

            this.clock.tick(1000 * 60); // +1 min, 6:30s
            expect(solenoids[0].on.calledOnce).to.be.true;
            expect(solenoids[0].off.calledOnce).to.be.true;
            expect(solenoids[1].on.calledOnce).to.be.true;
            expect(solenoids[1].off.calledOnce).to.be.true;
            expect(solenoids[2].on.calledOnce).to.be.true;
            expect(solenoids[2].off.calledOnce).to.be.true;

        });
    });
});
